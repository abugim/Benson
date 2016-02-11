#include "malha_fechada.h"

Malha_Fechada::Malha_Fechada(){
    *referencia = 0;
}

Malha_Fechada::~Malha_Fechada(){
    delete this;
}

double Malha_Fechada::acao(){
    *referencia = onda->proximo_ponto();
    *controle = *referencia - *nivel_um;
    trava_seguranca();
    return *controle_saturado;
}

char* Malha_Fechada::reporte(double tempo){
    char* mensagem;
    sprintf(mensagem, "%d\t%lf\t%d\t%lf\t%d\t%lf\t%d\t%lf\t%d\t%lf\t%d\t%lf",
                    TEMPO, tempo,
                    NIVEL_UM, *nivel_um,
                    NIVEL_DOIS, *nivel_dois,
                    REF, *referencia,
                    ERRO, *controle,
                    ERRO_SAT, *controle_saturado);
    return mensagem;
}
