#include "controle_pid.h"

Controle_PID::Controle_PID (Param_Desempenho *param_desempenho, PID *pid, bool var) : Malha_Fechada(param_desempenho, var) {
    this->pid = pid;
    //  erro_anterior = new double(0);
    pid->set_var(erro, erro_anterior);
    if (pid->pi_d) pid->set_var_der(erro, erro_anterior);
    else pid->set_var_der(var_controle, var_controle_anterior);
}
Controle_PID::~Controle_PID () {
    delete pid;
    delete this;
}

double Controle_PID::acao() {
    double referencia_anterior = *referencia;

    *referencia = onda->proximo_ponto();

    flag_referencia_mudou = referencia_anterior != *referencia;
    if (referencia_anterior != *referencia) {
        *referencia_passada = referencia_anterior;
    }

    *erro = *referencia - *var_controle;
    pid->acao(*controle == *controle_saturado);
    *controle = *pid->acao_prop + *pid->acao_int + *pid->acao_der;
    trava_seguranca();
    return *controle_saturado;
}

void Controle_PID::att(stringstream *ss){
    onda->att(ss);
    pid->att(ss);
}

char* Controle_PID::reporte(double tempo) {
    analise_desempenho(tempo);
    mensagem = (char*) malloc(2048 * sizeof(char));
    *erro_anterior = *erro;
    *var_controle_anterior = *var_controle;
    sprintf(mensagem, "%lf|%lf,%lf,%lf,%lf,%lf,%lf|%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                    tempo, *erro, *pid->acao_prop, *pid->acao_int,*pid->acao_der,
                    *controle, *controle_saturado,
                    *nivel_um, *nivel_dois, *referencia,
                    flag_pico, *tempo_pico,
                    flag_pico, *sobre_sinal,
                    flag_subida, *tempo_subida,
                    flag_acomodacao, *tempo_acomodacao,
                    flag_erro_regime, *erro_regime);

    // *nivel_um = (*referencia - ((*referencia - *referencia_passada) * (exp(-(tempo - *tempo_variacao_referencia)/2) * cos((tempo - *tempo_variacao_referencia)))));
    // *nivel_dois = (*referencia - ((*referencia - *referencia_passada) * (exp(-(tempo - *tempo_variacao_referencia)*2) * cos((tempo - *tempo_variacao_referencia)))));
    return mensagem;
}
