#include "stdafx.h"
#include "librerie.h";
typedef struct {
	float x;
	float y;
} Point2D;

//Per il picking
//extern int selected;
extern int wh, ww;
#define SIZE_BUFF 512


extern vector<Point2D> Punti;


void processHits(GLint hits, GLuint buffer[], int *selected) {
	unsigned int ii, jj;
	GLuint names, *ptr;

	ptr = (GLuint *)buffer;

	for (ii = 0; ii < hits; ii++) { /*  for each hit  */
		names = *ptr;
		ptr += 3;
		for (jj = 0; jj < names; jj++) { /*  for each name */
			*selected = *ptr;
			ptr++;
		}
	}
}
void gl_select(int x, int y, int *selected) {

	GLuint selectBuf[SIZE_BUFF];
	GLint hits;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	//inizializzo il buffer per gli oggetti selezionati
	glSelectBuffer(SIZE_BUFF, selectBuf);
	//abilito il rendering GL_SELECT
	glRenderMode(GL_SELECT);
	//inizializzo lo stack degli id
	glInitNames();
	//aggiungo almeno id = 0
	glPushName(0);
	//lavoro con le marici di proiezione e salvo i dati nello stack
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//seleziono una regione attorno alla posizione (x,y) 
	//del puntatore 5x5
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	//reimposto le coordinate della finestra sul mondo
	gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh);
	//ridisegno gli oggetti assegnando un id ad ognuno
	for (int i = 0; i<Punti.size(); i++) {
		glPushName(i);
		glBegin(GL_POINTS);
		glVertex2f(Punti[i].x, Punti[i].y);
		glEnd();
	}
	//lavoro con le matrici di proiezione e riprendo
	//tutti i dati salvati nello stack
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	//ritorno al rendering normale
	hits = glRenderMode(GL_RENDER);
	//processo gli oggetti selezionati
	processHits(hits, selectBuf, selected);

}

// fine picking
