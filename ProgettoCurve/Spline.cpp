#include "stdafx.h"
#include "librerie.h"
#include "Spline.h"
typedef struct {
	float x;
	float y;
} Point2D;

typedef struct {
	float x;
	float y;
	float z;
} Point3D;
extern vector<Point2D> Punti;
extern vector<Point2D> DerivateMod;
extern vector<float> pesi;
extern int molte;
extern int pval;
extern int flagModMolte;
extern int selected;
extern int m;  //ordine della spline
extern int estnodi;
extern int flagC;
int localizza_intervallo_internodale(float t, float* Nodi)
{
	int a = m - 1;
	int b;

	if (flagC == 0)
		b = Punti.size();
	else
		b = Punti.size() + 2;

	
		if (selected>3) {
			if (flagModMolte == 1)
				b = Punti.size() + molte - 1;

		}
	
	int centro;
	while (b - a > 1) {
		centro = (a + b) / 2;
		if (t < Nodi[centro]) {
			b = centro;
		}
		else
		{
			a = centro;
		}

	}
	return a;
}
void DeBoor(float *t, float* Nodi, float *sx, float* sy, int dim)
{
	int i;
	float tstep = 1.0 / (float)(pval - 1);

	int N = Punti.size();
	Point3D *c = new Point3D[N+molte - 1];


	float tg = 0.0;
	int k;

	for (k = 0; k < pval; k++, tg += tstep)


	{

		int l = localizza_intervallo_internodale(tg, Nodi);

		int cont = -1;
		for (i = 0; i<Punti.size(); i++)
		{
			cont++;
			c[cont].x = pesi.at(i)*Punti.at(i).x;
			c[cont].y = pesi.at(i)*Punti.at(i).y;
			c[cont].z = pesi.at(i);

		if (flagModMolte == 1)
			{
				if (selected > 3 && selected < (int)Punti.size() - 3)
				{
					if (i == selected)
					{
						for (int jj = 0; jj<molte - 1; jj++)
						{
							cont++;
							c[cont].x = pesi.at(selected)*Punti.at(selected).x;
							c[cont].y = pesi.at(selected)*Punti.at(selected).y;
							c[cont].z = pesi.at(selected);

						}
					}
				}
			}

		}


		for (int j = 1; j < m; j++)
		{
			for (i = l - m + j + 1; i <= l; i++)
			{
				int ti = i;
				int timj = i + m - j;

				float den = Nodi[timj] - Nodi[ti];



				c[i].x = (c[i].x*(tg - Nodi[ti]) + (c[i - 1].x*(Nodi[timj] - tg))) / den;
				c[i].y = (c[i].y*(tg - Nodi[ti]) + (c[i - 1].y*(Nodi[timj] - tg))) / den;
				c[i].z = (c[i].z*(tg - Nodi[ti]) + (c[i - 1].z*(Nodi[timj] - tg))) / den;


			}
		}


		sx[k] = c[l].x/c[l].z;
		sy[k] = c[l].y/c[l].z;


	}
}





void DisegnaBaseSpline(float *Nodi, int dim, float **B) {

	float ti = 0;
	float dt = 1.0 / (float)(pval - 1);
	int j;
	//in ogni colonna la j-esima funzione base e in ogni riga la valutazione nei punti
	float *v = new float[dim];

	//valutazione di ciascuna funzione base per ogni valore del parametro t mediante le formule di Cooks
	for (int k = 0; k < pval; k++, ti += dt) {
		int l = localizza_intervallo_internodale(ti, Nodi);

		costruisci_Bspline(l, Nodi, ti, v, dim);


		for (j = 0; j < dim; j++)
			B[k][j] = v[j];

	}

	
	float weightSum = 0;
		for (int k = 0; k < pval; k++)
		{
			weightSum = 0;
			for (int j = 0; j < Punti.size(); j++)
			{
				weightSum = weightSum + pesi.at(j)* B[k][j];
			}

			for (int j = 0; j < Punti.size(); j++)
			{
				B[k][j] = pesi.at(j)* B[k][j] / weightSum;
			}

		}
}

void Partizione_nodale_estesa(float *t, float *Nodi)
{
	int k;  //k numero dei nodi veri;
	int i, cont;
	k = Punti.size() - m;
	
	//Nodi fittizi a sinistra

	if (estnodi == 0) {
		Nodi[m - 1] = 0;

		float stepN = t[2] - t[1];
		for (int i = m - 2; i >= 0; i--)
			Nodi[i] = Nodi[i + 1] - stepN;
	}
	else
	{
		for (unsigned int i = 0; i < m; i++)
			Nodi[i] = 0;
	}
	//Nodi veri
	
		cont = 2;
	
	for (i = m; i < m + k; i++) {
		Nodi[i] = t[cont];
		cont++;
	}
	//Nodi fittizi a destra

	if (estnodi == 0)
	{
		float stepN = t[2] - t[1];
		Nodi[m + k] = 1;
		for (int i = m + k + 1; i < 2 * m + k; i++)
			Nodi[i] = Nodi[i - 1] + stepN;
	}
	else
	{
		for (unsigned int i = m + k; i < 2 * m + k; i++)
			Nodi[i] = 1;
	}

	int NumeroNodi = 2 * m + k;


		if (flagModMolte == 1)
		{

			if (selected > 3 && selected < (int)Punti.size() - 3) {
				printf("Punto Selezionato in Nodi %d \n", selected);
				//if (PuntoSelezionato>5 &&  PuntoSelezionato<Punti.size()-1){    //Perchè il Nodo[0]=Nodo[1]=Nodo[2]=Nodo[3]=0 è legato ai
					//primi 3 vertici di controllo
				float* temp = new float[2 * m + k];
				for (int i = 0; i < 2 * m + k; i++)
					temp[i] = Nodi[i];

				int inodo = selected + 1;  //la funzione corrispondente a questo nodo subisce le modifiche

				for (int i = inodo; i <= inodo + molte - 1; i++)
					Nodi[i] = Nodi[inodo];

				int cont1 = 1;
				for (int i = inodo + molte; i < NumeroNodi + molte - 1; i++)
				{
					Nodi[i] = temp[inodo + cont1];
					cont1++;
				}

				for (int i = 0; i < NumeroNodi + molte - 1; i++)
					printf("Nodo[%d]-->%f \n", i, Nodi[i]);
			}
		}
	}



void costruisci_Bspline(int l, float* Nodi, float punto, float *v, int dim)
{

	//Input: l=estremo sinistro dell'intervallo a cui punto appartiente
	//       Nodi   Vettore dei nodi
	//		 punto:  punto in cui valutare le b-spline diverse da zero
	//                 con indice l-m+1,...l 
	//       v:   vettore in cui sono memorizzati i valori delle b-spline diverse da zero.

	int k, i, j;
	float d1, d2, beta;
	for (i = 0; i < dim; i++)
		v[i] = 0;

	v[l] = 1;

	for (i = 0; i < m - 1; i++) {

		float tmp = 0.0;

		for (j = l - i; j <= l; j++) {
			d1 = punto - Nodi[j];
			d2 = Nodi[i + j + 1] - punto;
			beta = v[j] / (d1 + d2);
			v[j - 1] = d2*beta + tmp;
			tmp = d1*beta;
		}
		v[l] = tmp;
	}
}