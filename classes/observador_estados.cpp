#include "observador_estados.h"

ObservadorEstados::ObservadorEstados (Param_Desempenho *param_desempenho, bool flag_var, stringstream *ss)
    : Malha_Fechada(param_desempenho, flag_var) {
    *ss >> L[0] >> L[1];
    nivel_um_estimado = new double(0);
    nivel_dois_estimado = new double(0);
    nivel_um_estimado_anterior = new double(0);
    nivel_dois_estimado_anterior = new double(0);
    G[0][0] = 0.9935; G[0][1] = 0;
    G[1][0] = 0.00656; G[1][1] = 0.9935;
    H[0] = 0.0296;
    H[1] = 0.0000963;
}

ObservadorEstados::~ObservadorEstados() {

}

double ObservadorEstados::acao(){
    Malha_Fechada::acao();
    *nivel_um_estimado = (G[0][0] * (*nivel_um_estimado_anterior))
                            + (G[0][1] * (*nivel_dois_estimado_anterior))
                            + (L[0] * (*nivel_dois - *nivel_dois_estimado_anterior))
                            + (H[0] * (*referencia));
    *nivel_dois_estimado = (G[1][0] * (*nivel_um_estimado_anterior))
                            + (G[1][1] * (*nivel_dois_estimado_anterior))
                            + (L[1] * (*nivel_dois - *nivel_dois_estimado_anterior))
                            + (H[1] * (*referencia));
    return *controle_saturado;
}

char* ObservadorEstados::reporte(double tempo) {
    analise_desempenho(tempo);
    mensagem = (char*) malloc(2048 * sizeof(char));
    sprintf(mensagem, "%lf|%lf,%lf|%lf,%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                tempo, *controle, *controle_saturado,
                 *nivel_um, *nivel_dois, *nivel_um_estimado_anterior, *nivel_dois_estimado_anterior,
                 flag_pico, *tempo_pico,
                 flag_pico, *sobre_sinal,
                 flag_subida, *tempo_subida,
                 flag_acomodacao, *tempo_acomodacao,
                 flag_erro_regime, *erro_regime);
    *nivel_um_estimado_anterior = *nivel_um_estimado;
    *nivel_dois_estimado_anterior = *nivel_dois_estimado;
    return mensagem;
}

void ObservadorEstados::att(stringstream *ss) {
    *ss >> L[0] >> L[1];
}
