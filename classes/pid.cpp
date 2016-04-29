#include "pid.h"

PID::PID(double kp, double ki, double kd, bool pi_d, short int filtro_integrativo, double talt) {
    this->kp = new double;
    *this->kp = kp;

    this->ki = new double;
    *this->ki = ki;

    this->kd = new double;
    *this->kd = kd;

    this->pi_d = pi_d;

    this->filtro_integrativo = filtro_integrativo;
    this->talt = new double;
    *this->talt = talt;

    this->acao_prop = new double;
    *this->acao_prop = 0;

    this->acao_int = new double;
    *this->acao_int = 0;

    this->acao_der = new double;
    *this->acao_der = 0;
}

PID::PID(stringstream *ss) {
    kp = new double;
    ki = new double;
    kd = new double;
    pi_d = pi_d;
    talt = new double;

    acao_prop = new double;
    *acao_prop = 0;

    acao_int = new double;
    *acao_int = 0;

    acao_der = new double;
    *acao_der = 0;

    *ss >> *kp >> *ki >> *kd >> *talt >> pi_d >> filtro_integrativo;
}

PID::~PID() {
    delete kp;
    delete ki;
    delete kd;
    delete talt;
    delete acao_prop;
    delete acao_int;
    delete acao_der;
}

void PID::att(stringstream *ss) {
	*ss >> *kp >> *ki >> *kd >> pi_d >> filtro_integrativo >> *talt;
}

void PID::acao(bool saturacao) {
    *acao_prop = *kp * (*var);
    if (!saturacao)
        *acao_int += *ki * 0.1 * (*var);
    *acao_der = (*kd * ((*var_der) - *(var_der_ant)))/0.1;
}

void PID::set_var(double *var, double *var_ant){
    this->var = var;
    this->var_ant = var_ant;
}
void PID::set_var_der(double *var_der, double *var_der_ant){
    this->var_der = var_der;
    this->var_der_ant = var_der_ant;
}
