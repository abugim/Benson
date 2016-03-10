#include "malha_fechada.h"
#include "math.h"
Malha_Fechada::Malha_Fechada(){
    this->referencia = new double;
    this->erro = new double;
}

//void Malha_Fechada::att(double param[]){}

Malha_Fechada::~Malha_Fechada(){
    delete this;
}

double Malha_Fechada::acao(){
    *(this->referencia) = onda->proximo_ponto();
    *(this->erro) = *(this->referencia) - *(this->nivel_um);
    *(this->controle) = *(this->erro);
    trava_seguranca();
    return *(this->controle_saturado);
}

char* Malha_Fechada::reporte(double tempo){
    sprintf(mensagem, "%lf|%lf,%lf|%lf,%lf,%lf",
                tempo,*(this->controle), *(this->controle_saturado),
                 *(this->nivel_um), *(this->nivel_dois), *(this->referencia));

    return mensagem;
}
