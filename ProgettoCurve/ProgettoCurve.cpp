// progettoCurve.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "librerie.h"
#include "Bezier.h"
#include "hermite.h"
#include "Spline.h"
#include "picking.h"
#include "parametrizzazione.h"
#include "GestioneStringhe.h"
#include "GestioneMenu.h"

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int M_I = 0;
int id1, id2;
int pval = 500;

int flagModMolte = 0;
int flagModPesi = 0;
int m = 4;
int estnodi = 0;
int flagC = 0;
int molte = 1;
void drawScene();
void resize(GLsizei, GLsizei);
void mouse(int, int, int, int);

int metodo = 0, visualizzaTg = 0, dim;
int mod_par_der = 0;
float Tens, Cont, Bias;
float*t;
// Per i menu

int submenu_Opzioni_I, menu_id, submenu_hermite, submenu_bezier;
string mod = "Modalita' di inserimento ";
int scelta_param;
int submenu_param;
int submenu_spline;
string tipo_param;

typedef struct {
	float x;
	float y;
} Point2D;


Point2D newPoint, zero;

vector <Point2D> Punti;
vector <Point2D> DerivateMod;
vector<float> pesi;

vector <int>FlagP;
GLsizei wh = 500, ww = 500;
int numPoint = 0;

int selected = -1;
int flagG = 0;







void drawScene() {

	string  NumeroPunti, s, coordXins, coordYins, stringaAscIns, stringaOrdIns;
	string coordinate, xpS, ypS, ascissa, ordinata;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(6.0);
	for (int i = 0; i<Punti.size(); i++) {
		if (FlagP[i] == 0) {
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_POINTS);
			glVertex2f(Punti[i].x, Punti[i].y);
			glEnd();
		}
		else
		{
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_POINTS);
			glVertex2f(Punti[i].x, Punti[i].y);
			glEnd();
		}

	}
	if (selected > -1)
	{

		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		glVertex2f(Punti[selected].x, Punti[selected].y);
		glEnd();
		glColor3f(1.0, 0.0, 0.0);
		FlagP[selected] = 1;

	}

	//abilito la modalità di disegno primitive in maniera tratteggiata

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(4, 0x3F07);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i<Punti.size(); i++)
		glVertex2f(Punti[i].x, Punti[i].y);
	glEnd();
	//abilito la modalità di sisegno primitive in maniera tratteggiata
	glDisable(GL_LINE_STIPPLE);

	if (metodo == 1)
		dim = Punti.size() + 1;
	else
		dim = Punti.size();

	float**B = new float*[pval];
	for (int i = 0; i < pval; i++)
		B[i] = new float[dim];
	t = new float[Punti.size()];
	if (scelta_param == 0)
		parametrizzazione_uniforme(t);
	else
		parametrizzazione_corde(t);



	if (Punti.size()>2)
	{

		if (metodo == 0)
		{



			float *cx = new float[pval];
			float *cy = new float[pval];
			CostruisciBezier(cx, cy);

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < pval - 1; i++)
				glVertex2f(cx[i], cy[i]);

			glEnd();

			DisegnaBasebezier(B);
		}

		if (metodo == 1)
		{
			float *px = new float[pval];
			float *py = new float[pval];

			InterpolazioneHermite(t, px, py);
			glColor3f(0.0, 0.0, 1.0);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < pval; i++)
				glVertex3f(px[i], py[i], 0);
			glEnd();

		}



	}

	if (metodo == 2)
	{
		if (Punti.size() >= m) {


			int DimPartNodEstesa;

			DimPartNodEstesa = Punti.size() + m + molte - 1;

			float *Nodi = new float[DimPartNodEstesa];

			float *sx = new float[pval];
			float *sy = new float[pval];

			float *x = new float[Punti.size()];
			float *y = new float[Punti.size()];


			/*for (int i = 0; i < Punti.size(); i++)
			{
				x[i] = Punti[i].x;
				y[i] = Punti[i].y;


			}*/
			Partizione_nodale_estesa(t, Nodi);
			for (int i = 0; i < DimPartNodEstesa; i++)
				printf("Nodi[%d]=%f\n", i, Nodi[i]);

			DeBoor(t, Nodi, sx, sy, dim);
			

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < pval; i++)
				glVertex3f(sx[i], sy[i], 0);
			glEnd();

			DisegnaBaseSpline(Nodi, dim, B);

		}
	}

	glFlush();




	//Sulla finestra delle informazioni

	glutSetWindow(id2);



	glClear(GL_COLOR_BUFFER_BIT);

	if (M_I == 0)
	{
		if (Punti.size() >= 0)
		{
			stringaAscIns = "Ascissa punto inserito ";
			stringaOrdIns = "Ordinata punto inserito ";
			coordXins = floatToString(newPoint.x);
			coordYins = floatToString(newPoint.y);
			stringaAscIns.append(coordXins.substr(0, 7));
			stringaOrdIns.append(coordYins.substr(0, 7));
			bitmap_output(10, 50, 0, stringaAscIns, GLUT_BITMAP_HELVETICA_18);
			bitmap_output(10, 70, 0, stringaOrdIns, GLUT_BITMAP_HELVETICA_18);
		}
	}

	//Scrivo il numero di punti sulla finestra
	NumeroPunti = intToString(Punti.size());
	s = "Numero di punti : ";
	s.append(NumeroPunti);
	glColor3f(1.0, 0.0, 0.0);
	bitmap_output(10, 30, 0, s, GLUT_BITMAP_HELVETICA_18);

	//Scrivo la modalità della finestra
	glColor3f(1.0, 1.0, 0.0);
	bitmap_output(200, 30, 0, mod, GLUT_BITMAP_HELVETICA_18);

	if (metodo == 0 || metodo == 2)
	{
		glPushMatrix();
		glTranslatef(50, 150, 0);
		glScalef(400, 300, 1);
		for (int k = 0; k < Punti.size(); k++)
		{
			float ti = 0;
			float dt = 1.0 / (float)(pval - 1);
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < pval; i++, ti += dt)
				glVertex3f(ti, B[i][k], 0.0);
			glEnd();

		}
		glPopMatrix();


	}

	glFlush();

}


void mykeyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'T':
		Tens += 0.1;
		break;
	case 't':
		Tens -= 0.1;
		break;

	case 'C':
		Cont += 0.1;
		break;
	case 'c':
		Cont -= 0.1;
		break;
	case 'B':
		Bias += 0.1;
		break;
	case 'b':
		Bias -= 0.1;
		break;

		//Modifica molteplicita dei nodi
	case '1':
		molte = 1;
		break;

	case '2':
		molte = 2;
		break;

	case '3':
		molte = 3;
		break;

		//Modifica pesi nei vertici di controllo

	case '+':
		pesi.at(selected) += 0.1;
		break;

	case '-':
		pesi.at(selected) -= 0.1;

		if (pesi.at(selected) <= 0)
			pesi.at(selected) = 0.01;

		break;


	}
	DerivateMod.at(selected).x = dx(selected, t, Tens, Bias, Cont);
	DerivateMod.at(selected).y = dy(selected, t, Tens, Bias, Cont);




	glutPostRedisplay();

}


void mouse(int button, int state, int x, int y) {


	newPoint.x = float(x);
	newPoint.y = wh - float(y);
	zero.x = 0.0;
	zero.y = 0.0;
	if (state == GLUT_DOWN)
	{
		switch (button)
		{


		case GLUT_LEFT_BUTTON:
			//se viene premuto il pulsante sinistro del mouse e Modalità inserimento punti (MI=0)
			if (M_I == 0)
			{
				Punti.push_back(newPoint);
				DerivateMod.push_back(zero);
				pesi.push_back(1.0);
				FlagP.push_back(0);
			}

			//se viene premuto il pulsante sinsitro del mouse  in  Modalità Modifica punti (MI=1)

			if (M_I == 1 || mod_par_der == 1 || flagModPesi || flagModMolte)
			{



				//... individuo l'oggetto selezionato dall'utente 
				//(select conterrà l'id dell'punto relativo alla lista) ...
				gl_select(x, y, &selected);
				//... se l'utente ha selezionato un punto ...
				if (selected >= 0) {
					printf("Punto Selezionato: %d\n", selected);
				}

				Tens = 0.0;
				Cont = 0.0;
				Bias = 0.0;
			}

			//se viene premuto il pulsante sinsitro del mouse  in  Modalità Elimina punti (Mi=2)
			if (M_I == 2)
			{
				Punti.pop_back();
			}

			break;
		}


	}
	glutPostRedisplay();
}

/* funzioni per la fase di picking */

void resize(GLsizei w, GLsizei h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	ww = w;
	wh = h;
}
void mouseMotion(int x, int y)
{
	Point2D newPoint;
	newPoint.x = x;
	newPoint.y = 500 - y;

	if (selected >= 0)
	{
		if (M_I == 1)
		{
			Punti.at(selected) = newPoint;


		}
	}
	glutPostRedisplay();
}


void  main(int argc, char **argv)
{

	GLboolean GlewInitResult;

	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(600, 1);
	id1 = glutCreateWindow("Editor Curve");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(mykeyboard);
	createMenu();

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 1);
	id2 = glutCreateWindow("Informazioni");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);



	glutMainLoop();
}

