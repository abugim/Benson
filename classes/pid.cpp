#include "pid.h"

Controle_PID::Controle_PID(double kp, double ki, double kd, bool pi_d, short int filtro){
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

    this->talt = new double;
    *(this->talt) = sqrt(*(this->kd)/(*(this->ki)));

    this->pi_d = pi_d;
    this->filtro = filtro;

    this->acao_prop = new double;
    *(this->acao_prop) = 0;

    this->acao_int = this->integrador;

    this->acao_der = new double;
    *(this->acao_der) = 0;
}

Controle_PID::~Controle_PID(){
    delete onda;
    delete this;
}

void Controle_PID::att(double param[]){
    printf("kp\n");
    *(this->kp) = param[0];
    printf("ki\n");
    *(this->ki) = param[1];
    printf("kd\n");
    *(this->kd) = param[2];
    printf("pi-d\n");
    this->pi_d = param[3];
    printf("filtro\n");
    this->filtro = param[4];
}

void Controle_PID::back(){

}

double Controle_PID::acao(){
    *(this->referencia) = this->onda->proximo_ponto();
    *(this->erro) = *(this->referencia) - *(this->nivel_um);
    *(this->controle) = acaoP() + acaoI() + acaoD();
    trava_seguranca();
    return *(this->controle_saturado);
}

char* Controle_PID::reporte(double tempo){
    sprintf(mensagem, "%lf|%lf,%lf,%lf,%lf,%lf,%lf|%lf,%lf,%lf",
                tempo, *(this->erro), *(this->acao_prop), *(this->acao_int),
                *(this->acao_der), *(this->controle), *(this->controle_saturado),
                *(this->nivel_um), *(this->nivel_dois), *(this->referencia));
    return mensagem;
}

double Controle_PID::acaoP(){
    *(this->acao_prop) = *(this->kp) * (*(this->erro));
    return *(this->acao_prop);
}

double Controle_PID::acaoI(){
    if (this->filtro == COND_FILTRO){
        if (((*(this->controle_saturado) == 4) &&  (*(this->erro) > 0))
            || ((*(this->controle_saturado) == -4) &&  (*(this->erro) < 0))){
            return *(this->integrador);
        }
    } else if (this->filtro == BACK_FILTRO) {
        *(this->integrador) += *(this->ki) * 0.1 * (*(this->erro)) +
                            (*(this->controle_saturado) - *(this->controle)) / *(this->talt);
    } else {
        *(this->integrador) += *(this->ki) * 0.1 * (*(this->erro));
    }
    return *(this->integrador);
}

double Controle_PID::acaoD(){
    if (pi_d)
        *(this->acao_der) = *(this->kd) * (*(this->nivel_um) - *(this->nivel_um_anterior))/0.1;
    else
        *(this->acao_der) = *(this->kd) * (*(this->erro) - *(this->erro_anterior))/0.1;
    return *(this->acao_der);
}
