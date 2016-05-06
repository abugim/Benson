#include "stalker.h"

Stalker::Stalker (Param_Desempenho *param_desempenho, bool flag_var, stringstream *ss)
    : Malha_Fechada(param_desempenho, flag_var) {
        *ss >> K[0] >> K[1] >>  K[2];
        v_ant = 0;
}

Stalker::~Stalker(){

}

double Stalker::acao(){
    *erro = *referencia - *nivel_dois;
    *controle = (K[2] * (*erro - v_ant)) - (K[0] * *nivel_um) - (K[1] * *nivel_dois);
    v_ant = *erro - v_ant;
    trava_seguranca();
    return *controle_saturado;
}

char* Stalker::reporte(double tempo) {
    analise_desempenho(tempo);
    mensagem = (char*) malloc(2048 * sizeof(char));
    sprintf(mensagem, "%lf|%lf,%lf,%lf|%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                tempo, *controle, *controle_saturado, *erro,
                 *nivel_um, *nivel_dois, *referencia,
                 flag_pico, *tempo_pico,
                 flag_pico, *sobre_sinal,
                 flag_subida, *tempo_subida,
                 flag_acomodacao, *tempo_acomodacao,
                 flag_erro_regime, *erro_regime);
    *erro_anterior = *erro;
    return mensagem;
}

void Stalker::att(stringstream *ss) {
    *ss >> K[0] >> K[1] >>  K[2];
}
