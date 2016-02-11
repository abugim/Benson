#include "malha_fechada.h"

Malha_Fechada::Malha_Fechada(){

}

Malha_Fechada::~Malha_Fechada(){

}

double Controle::acao(){
    *referencia = onda->proximo_ponto();
    *controle = *referencia - *nivel_um;
    trava_seguranca();
    return *controle_saturado;
}

char* Controle::reporte(double tempo){
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
