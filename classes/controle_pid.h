#ifndef _CONTROLE_PID_H
#define _CONTROLE_PID_H

#include "malha_fechada.h"
#include "pid.h"

class Controle_PID : public Malha_Fechada {
private:
    /* data */
public:
    Controle_PID (Param_Desempenho *param_desempenho, PID *pid, bool var);
    virtual ~Controle_PID ();

    void att(stringstream *ss);
    double acao();
    char* reporte(double tempo);

    void back();

protected:
    PID *pid;
    // double *erro_anterior;
};

#endif
