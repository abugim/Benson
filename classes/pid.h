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

class PID {
public:
    PID(stringstream *ss);
    PID(double kp, double ki, double kd, bool pi_d, short int filtro_integrativo, double talt);
    virtual ~PID();

    void acao(bool saturacao);

    void set_var(double *var);
    void set_var_der(double *var_der, double *var_der_ant);
    void att(stringstream *ss);

public:
    // Flag para determinar se é PID ou PI-D
    double *kp;
    double *ki;
    double *kd;

    bool pi_d;
    short int filtro_integrativo;
    double *talt;

    double *acao_prop;
    double *acao_int;
    double *acao_der;

    double *var;
    double *var_der;
    double *var_der_ant;
};

#endif
