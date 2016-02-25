#include "pid.h"

Controle_PID::Controle_PID(double kp, double ki, double kd, bool pi_d){
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

Controle_PID::~Controle_PID(){
    delete onda;
    delete this;
}

double Controle_PID::acao(){
    *(this->referencia) = this->onda->proximo_ponto();
    *(this->erro) = *(this->referencia) - *(this->nivel_um);
    *(this->controle) = acaoP() + acaoI() + acaoD();
    trava_seguranca();
    return *(this->controle_saturado);
}

char* Controle_PID::reporte(double tempo){
    sprintf(mensagem, "%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf\n",
                    TEMPO, tempo,
                    NIVEL_UM, *(this->nivel_um),
                    NIVEL_DOIS, *(this->nivel_dois),
                    REF, *(this->referencia),
                    ERRO, *(this->erro),
                    CONTROLE, *(this->controle),
                    ACAOP, *(this->acao_prop),
                    ACAOI, *(this->acao_int),
                    ACAOD, *(this->acao_der),
                    CONTROLE_SATURADO, *(this->controle_saturado));
    return mensagem;
}

double Controle_PID::acaoP(){
    *(this->acao_prop) = *(this->kp) * (*(this->erro));
    return *(this->acao_prop);
}

double Controle_PID::acaoI(){
    *(this->integrador) += *(this->ki) * 0.1 * (*(this->erro));
    return *(this->integrador);
}

double Controle_PID::acaoD(){
    if (pi_d)
        *(this->acao_der) = *(this->kd) * (*(this->nivel_um) - *(this->nivel_um_anterior))/0.1;
    else
        *(this->acao_der) = *(this->kd) * (*(this->erro) - *(this->erro_anterior))/0.1;
    return *(this->acao_der);
}
