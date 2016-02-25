#include "pid.h"

PID::PID(double kp, double ki, double kd, bool pi_d){
    this->integrador = new double;
    *(this->integrador) = 0;

    this->erro_anterior = new double;
    *(this->erro_anterior) = 0;

    this->nivel_um_anterior = new double;
    *(this->nivel_um_anterior) = 0;

    this->kp = new double;
    *(this->kp) = kp;

    this->ki = new double;
    *(this->ki) = ki;

    this->kd = new double;
    *(this->kd) = kd;

    this->pi_d = pi_d;
}

PID::~PID(){
    delete onda;
    delete this;
}

double acao(){
    *(this->referencia) = onda->proximo_ponto();
    *(this->erro) = *(this->referencia) - *(this->nivel_um);
    *(this->controle) = acaoP(this->erro) + acaoI(this->erro) + acaoD(this->erro)
    trava_seguranca();
    return *(this->controle_saturado);
}

char* PID::reporte(double tempo){
    sprintf(mensagem, "%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf\n",
                    TEMPO, tempo,
                    NIVEL_UM, *(this->nivel_um),
                    NIVEL_DOIS, *(this->nivel_dois),
                    REF, *(this->referencia),
                    ERRO, *(this->erro),
                    CONTROLE, *(this->controle),
                    ACAOP, *(this->acaoP),
                    ACAOI, *(this->acaoI),
                    ACAOD, *(this->acaoD),
                    CONTROLE_SATURADO, *(this->controle_saturado));
    return mensagem;
}

double PID::acaoP(double &erro){
    return *(this->kp) * erro;
}

double PID::acaoI(double &erro){
    *(this->integrador) += *(this->ki) * 0.1 * erro;
    return *(this->integrador);
}

double PID::acaoD(double &erro){
    if (pi_d)
        double acaoD = kd * (nivel_um - *(this->nivel_um_anterior))/0.1;
    else
        double acaoD = kd * (erro - *(this->erro_anterior))/0.1;
    return acaoD;
}
