#ifndef _PID_H
#define _PID_H

#include "malha_fechada.h"

#define ACAOP 8
#define ACAOI 9
#define ACAOD 10
#define CONTROLE 11
#define CONTROLE_SATURADO 12

class PID : public Malha_Fechada {
public:
    PID(double kp, double ki, double kd, double pi_d);
    virtual ~PID();
    double acao();
    char* reporte(double tempo);

    double acaoP(double &erro);
    double acaoI(double &erro);
    double acaoD(double &erro);
private:
    // Flag para determinar se é PID ou PI-D
    bool pi_d;

    double *integrador;
    double *erro_anterior;
    double *nivel_um_anterior;

    double *kp;
    double *ki;
    double *kd;
}

#endif
