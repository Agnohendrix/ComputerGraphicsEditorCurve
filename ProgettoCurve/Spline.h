

int localizza_intervallo_internodale(float t, float* Nodi);
void DeBoor(float *t, float* Nodi, float *sx, float* sy, int dim);
void DisegnaBaseSpline(float *Nodi, int dim, float **B);
void Partizione_nodale_estesa(float *t, float *Nodi);
void costruisci_Bspline(int l, float* Nodi, float punto, float *v, int dim);
