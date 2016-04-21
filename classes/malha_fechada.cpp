#include "malha_fechada.h"

Malha_Fechada::Malha_Fechada (Param_Desempenho *param_desempenho, bool flag_var) {
    this->var_controle = flag_var ? nivel_um : nivel_dois;

    this->var_controle_anterior = new double;
    *this->var_controle_anterior = 0;

    this->referencia = new double;
    *this->referencia = 0;

    this->referencia_passada = new double;
    *this->referencia_passada = 0;

    this->erro = new double;
    *this->erro = 0;
    this->param_desempenho = param_desempenho;
}

Malha_Fechada::~Malha_Fechada(){
    delete var_controle;
    delete var_controle_anterior;
    delete referencia;
    delete referencia_passada;
    delete erro;
    delete param_desempenho;
    delete tempo_variacao_referencia;
    delete tempo_subida;
    delete tempo_subida_inicio;
    delete tempo_subida_final;
    delete tempo_acomodacao;
    delete tempo_pico;
    delete sobre_sinal;
    delete erro_regime;
    delete this;
}

double Malha_Fechada::acao(){
    *referencia = onda->proximo_ponto();
    *erro = *referencia - *var_controle;
    *controle = *erro;
    trava_seguranca();
    return *controle_saturado;
}

char* Malha_Fechada::reporte(double tempo){
    this->mensagem =  (char*) malloc(2048 * sizeof(char));
    sprintf(mensagem, "%lf|%lf,%lf|%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                tempo, *controle, *controle_saturado,
                 *nivel_um, *nivel_dois, *referencia,
                 flag_pico, *tempo_pico,
                 flag_pico, *sobre_sinal,
                 flag_subida, *tempo_subida,
                 flag_acomodacao, *tempo_acomodacao,
                 flag_erro_regime, *erro_regime);
    return mensagem;
}

void Malha_Fechada::att(stringstream *ss) {
    onda->att(ss);
}

void Malha_Fechada::analise_desempenho(double tempo) {
    if (onda->get_tipo() == DEGRAU || onda->get_tipo() == QUADRADA || onda->get_tipo() == ALEATORIO) {
        if (flag_referencia_mudou) {
            referencia_mudou(tempo);
        }
        if (!flag_subida) {
            analise_tempo_subida(tempo);
        }
        analise_tempo_acomodacao(tempo);
        analise_pico(tempo);
        analise_erro();
    }
}

void Malha_Fechada::analise_tempo_subida(double tempo) {
    double variacacao_referencia = *referencia - *referencia_passada;
    if (*referencia - *referencia_passada > 0){
			if(!flag_subida_inicio && *var_controle >= *referencia_passada + (param_desempenho->fator_subida) * variacacao_referencia)
			{
				*tempo_subida_inicio = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_inicio = true;
			}
			if (!flag_subida_final && *var_controle >= *referencia_passada + (1 - param_desempenho->fator_subida) * variacacao_referencia){
				*tempo_subida_final = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_final = true;
			}
			if (flag_subida_inicio && flag_subida_final)
			{
				*tempo_subida = *tempo_subida_final - *tempo_subida_inicio;
				flag_subida = true;
			}
		}else{
			if(!flag_subida_inicio && *var_controle <= *referencia_passada + (param_desempenho->fator_subida) * variacacao_referencia)
			{
				*tempo_subida_inicio = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_inicio = true;
			}
			if (!flag_subida_final && *var_controle <= *referencia_passada + (1 - param_desempenho->fator_subida) * variacacao_referencia){
				*tempo_subida_final = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_final = true;
			}
			if (flag_subida_inicio && flag_subida_final)
			{
				*tempo_subida = *tempo_subida_final - *tempo_subida_inicio;
				flag_subida = true;
			}
		}
}

void Malha_Fechada::analise_tempo_acomodacao(double tempo) {
    if(!flag_acomodacao) {
        if(*var_controle <= *referencia * (1 + param_desempenho->fator_acomodacao) && *var_controle >= *referencia * (1 - param_desempenho->fator_acomodacao)) {
            *tempo_acomodacao = tempo - *tempo_variacao_referencia - 0.1;
            flag_acomodacao = true;
        }
    } else {
        if((*var_controle >= *referencia * (1 + param_desempenho->fator_acomodacao)) || (*var_controle <= *referencia * (1 - param_desempenho->fator_acomodacao))) {
            flag_acomodacao = false;
        }
    }
}

void Malha_Fechada::analise_pico(double tempo) {
    double variacacao_referencia = *referencia - *referencia_passada;
    if(!flag_pico){
        if (variacacao_referencia > 0) {
            if (*var_controle < *var_controle_anterior && *var_controle > *referencia) {
                *tempo_pico = tempo - *tempo_variacao_referencia - 0.2;
                if (!param_desempenho->unidade_sobressinal){
                    *sobre_sinal = fabs(100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada));
                } else {
                    *sobre_sinal = fabs(*var_controle_anterior - *referencia);
                }
                flag_pico = true;
            }
        } else {
            if (*var_controle > *var_controle_anterior && *var_controle < *referencia) {
                *tempo_pico = tempo - *tempo_variacao_referencia - 0.2;
                if (!param_desempenho->unidade_sobressinal){
                    *sobre_sinal = fabs(100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada));
                } else {
                    *sobre_sinal = fabs(*var_controle_anterior - *referencia);
                }
                flag_pico = true;
            }
        }
        if (variacacao_referencia > 0) {
            if (*var_controle > *referencia * (1 + *sobre_sinal)) {
                flag_pico = false;
            }
        } else {
            if (*var_controle < (param_desempenho->unidade_sobressinal ? *referencia - *sobre_sinal : *referencia * (1 - *sobre_sinal))) {
                flag_pico = false;
            }
        }
    }
}

void Malha_Fechada::referencia_mudou(double tempo) {
    flag_referencia_mudou = false;
    *tempo_variacao_referencia = tempo - 0.1;
    flag_pico = flag_acomodacao = flag_subida = false;
    flag_subida_inicio = flag_subida_final = false;
}

void Malha_Fechada::analise_erro() {
     if ((*var_controle - *var_controle_anterior) < ERRO_RAND) {
         flag_erro_regime = true;
         *erro_regime = *referencia - *var_controle;
     }
}
