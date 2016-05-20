#ifndef _OE_H
#define _OE_H

#include "malha_fechada.h"
#include <math.h>

class ObservadorEstados : public Malha_Fechada {
public:
    ObservadorEstados (Param_Desempenho *param_desempenho, bool flag_var, stringstream *ss);
    virtual ~ObservadorEstados ();
    double acao();
    char* reporte(double tempo);
    void att(stringstream *ss);

private:
    double *nivel_dois_anterior;
    double G[2][2];
    double L[2];
    double H[2];
    double *nivel_um_estimado;
    double *nivel_dois_estimado;
    double *nivel_um_estimado_anterior;
    double *nivel_dois_estimado_anterior;
};

#endif
