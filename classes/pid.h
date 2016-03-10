#ifndef _PID_H
#define _PID_H

#include "malha_fechada.h"

#define ACAOP 8
#define ACAOI 9
#define ACAOD 10
#define CONTROLE 11
#define CONTROLE_SATURADO 12

#define SEM__FILTRO 0
#define BACK_FILTRO 1
#define COND_FILTRO 2

class Controle_PID : public Malha_Fechada {
public:
    Controle_PID(double kp, double ki, double kd, bool pi_d, short int filtro);
    virtual ~Controle_PID();
    double acao();
    char* reporte(double tempo);

    void att(double param[]);
    double acaoP();
    double acaoI();
    double acaoD();

    void back();

private:
    // Flag para determinar se é PID ou PI-D
    bool pi_d;

    double *integrador;
    double *erro_anterior;
    double *nivel_um_anterior;

    double *kp;
    double *ki;
    double *kd;

    double *talt;

    double *acao_prop;
    double *acao_int;
    double *acao_der;

    short int filtro;
};

#endif
