#ifndef _CACHOEIRA_H
#define _CACHOEIRA_H

#include "malha_fechada.h"
#include "param_desempenho.h"
#include "pid.h"


class ControleCachoeira : public Malha_Fechada {
private:
    PID *mestre;
    PID *escravo;

    double *nivel_um_anterior;
    double *nivel_dois_anterior;

    double *erro_escravo;
    double *erro_escravo_anterior;
    double *controle_mestre;
public:
    ControleCachoeira (Param_Desempenho *param, PID *escravo, PID *mestre);
    virtual ~ControleCachoeira ();

    double acao();
    char* reporte(double tempo);
    void att(stringstream *ss);
};

#endif
