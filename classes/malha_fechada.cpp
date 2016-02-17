#include "malha_fechada.h"

Malha_Fechada::Malha_Fechada(){
    this->referencia = new double;
}

Malha_Fechada::~Malha_Fechada(){
    delete this;
}

double Malha_Fechada::acao(){
    *(this->referencia) = onda->proximo_ponto();
    *(this->controle) = *(this->referencia) - *(this->nivel_um);
    trava_seguranca();
    return *(this->controle_saturado);
}

char* Malha_Fechada::reporte(double tempo){
    char* mensagem;
    sprintf(mensagem, "%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf|%d|%lf\n",
                    TEMPO, tempo,
                    NIVEL_UM, *(this->nivel_um),
                    NIVEL_DOIS, *(this->nivel_dois),
                    REF, *(this->referencia),
                    ERRO, *(this->controle),
                    ERRO_SAT, *(this->controle_saturado));

    return mensagem;
}
