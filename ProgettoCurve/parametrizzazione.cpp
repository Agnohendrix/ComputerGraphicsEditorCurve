//Parametrizzazione unifome
#include "stdafx.h"
#include "librerie.h"
typedef struct {
	float x;
	float y;
} Point2D;

extern vector<Point2D> Punti;

void parametrizzazione_uniforme(float *t)
{
	int i;
	float step = 1.0 / (float)(Punti.size() - 1);

	for (i = 0; i < Punti.size(); i++)
		t[i] = i*step;

}

//Parametrizzazione secondo la lunghezza della corda

void parametrizzazione_corde(float *t)
{
	int i;
	t[0] = 0;

	for (i = 1; i < Punti.size(); i++)
		t[i] = t[i - 1] + sqrt((Punti.at(i).x - Punti.at(i - 1).x)*(Punti.at(i).x - Punti.at(i - 1).x) + (Punti.at(i).y - Punti.at(i - 1).y)*(Punti.at(i).y - Punti.at(i - 1).y));

	for (i = 1; i < Punti.size(); i++)
		t[i] = t[i] / t[Punti.size() - 1];


}
