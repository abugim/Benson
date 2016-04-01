#include "pid.h"

Controle_PID::Controle_PID(double kp, double ki, double kd, bool pi_d, short int filtro, float fator_subida, float fator_acomodacao, bool unidade_sobressinal, bool flag_var_controle) {
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
    *(this->acao_int) = 0;

    this->acao_der = new double;
    *(this->acao_der) = 0;

    this->referencia_passada = new double;
    *this->referencia_passada = 0;

    this->tempo_variacao_referencia = new double;
    *this->tempo_variacao_referencia = 0;

    this->unidade_sobressinal = unidade_sobressinal;

    this->fator_subida = new double;
    *this->fator_subida = fator_subida;

    this->fator_acomodacao = new double;
    *this->fator_acomodacao = fator_acomodacao;

    this->tempo_subida = new double;
    *(this->tempo_subida) = 0;

    this->tempo_subida_inicio = new double;
    *(this->tempo_subida_inicio) = 0;

    this->tempo_subida_final = new double;
    *(this->tempo_subida_final) = 0;

    this->tempo_acomodacao = new double;
    *(this->tempo_acomodacao) = 0;

    this->tempo_pico = new double;
    *(this->tempo_pico) = 0;

    this->sobre_sinal = new double;
    *(this->sobre_sinal) = 0;

    this->flag_var_controle = flag_var_controle;

    this->var_controle_anterior = new double;
    *this->var_controle_anterior = 0;

    this->flag_referencia_mudou = false;
}

Controle_PID::~Controle_PID() {
    delete onda;
    delete this;
}

void Controle_PID::att(double param[]) {
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

    *(this->talt) = sqrt(*(this->kd)/(*(this->ki)));
}

double Controle_PID::acao() {
    double referencia_anterior = *this->referencia;

    *this->referencia = this->onda->proximo_ponto();

    flag_referencia_mudou = referencia_anterior != *referencia;
    if (referencia_anterior != *referencia) {
        *referencia_passada = referencia_anterior;
    }

    if (!flag_var_controle) {
        var_controle = nivel_um;
    } else {
        var_controle = nivel_dois;
    }

    *erro = *referencia - *var_controle;
    *controle = acaoP() + acaoI() + acaoD();
    trava_seguranca();
    return *controle_saturado;
}

char* Controle_PID::reporte(double tempo) {
    analise_desempenho(tempo);

    *var_controle_anterior = *var_controle;

    sprintf(mensagem, "%lf|%lf,%lf,%lf,%lf,%lf,%lf|%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                tempo, *(this->erro), *(this->acao_prop), *(this->acao_int),
                *(this->acao_der), *(this->controle), *(this->controle_saturado),
                *(this->nivel_um), *(this->nivel_dois), *(this->referencia),
                flag_pico, *tempo_pico, flag_pico, *sobre_sinal,
                flag_subida, *tempo_subida, flag_acomodacao, *tempo_acomodacao);

    *nivel_um = (*referencia - ((*referencia - *referencia_passada) * (exp(-(tempo - *tempo_variacao_referencia)/2) * cos((tempo - *tempo_variacao_referencia)))));
    *nivel_dois = (*referencia - ((*referencia - *referencia_passada) * (exp(-(tempo - *tempo_variacao_referencia)*2) * cos((tempo - *tempo_variacao_referencia)))));
    return mensagem;
}

double Controle_PID::acaoP() {
    *(this->acao_prop) = *(this->kp) * (*(this->erro));
    return *(this->acao_prop);
}

double Controle_PID::acaoI() {
    if (this->filtro == COND_FILTRO){
        if (((*(this->controle_saturado) == 4) &&  (*(this->erro) > 0))
            || ((*(this->controle_saturado) == -4) &&  (*(this->erro) < 0))){
            return *(this->integrador);
        } else {
            *(this->integrador) += *(this->ki) * 0.1 * (*(this->erro));
            return *(this->integrador);
        }
    } else if (this->filtro == BACK_FILTRO) {
        *(this->integrador) += 0.1 * (*(this->ki) * (*(this->erro)) +
                            (*(this->controle_saturado) - *(this->controle)) / *(this->talt));
    } else {
        *(this->integrador) += *(this->ki) * 0.1 * (*(this->erro));
    }
    return *(this->integrador);
}

double Controle_PID::acaoD() {
    if (pi_d) {
        *(this->acao_der) = *(this->kd) * (*(this->var_controle) - *(this->var_controle_anterior))/0.1;
        *(this->var_controle_anterior) = *(this->var_controle);
    } else {
        *(this->acao_der) = *(this->kd) * (*(this->erro) - *(this->erro_anterior))/0.1;
        *(this->erro_anterior) = *(this->erro);
    }
    return *(this->acao_der);
}

void Controle_PID::analise_desempenho(double tempo) {
    if (flag_referencia_mudou) {
        referencia_mudou(tempo);
    }
    if (!flag_subida) {
        analise_tempo_subida(tempo);
    }
    analise_tempo_acomodacao(tempo);
    analise_pico(tempo);
}

void Controle_PID::analise_tempo_subida(double tempo) {
    double variacacao_referencia = *referencia - *referencia_passada;
    if (*referencia - *referencia_passada > 0){
			if(!flag_subida_inicio && *var_controle >= *referencia_passada + (*fator_subida) * variacacao_referencia)
			{
				*tempo_subida_inicio = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_inicio = true;
			}
			if (!flag_subida_final && *var_controle >= *referencia_passada + (1 - *fator_subida) * variacacao_referencia){
				*tempo_subida_final = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_final = true;
			}
			if (flag_subida_inicio && flag_subida_final)
			{
				*tempo_subida = *tempo_subida_final - *tempo_subida_inicio;
				flag_subida = true;
			}
		}else{
			if(!flag_subida_inicio && *var_controle <= *referencia_passada + (*(fator_subida)) * variacacao_referencia)
			{
				*tempo_subida_inicio = tempo - *tempo_variacao_referencia - 0.2;
				flag_subida_inicio = true;
			}
			if (!flag_subida_final && *var_controle <= *referencia_passada + (1 - *fator_subida) * variacacao_referencia){
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

void Controle_PID::analise_tempo_acomodacao(double tempo) {
    if(!flag_acomodacao) {
        if(*var_controle <= *referencia * (1 + (*fator_acomodacao)) && *var_controle >= *referencia * (1 - (*fator_acomodacao))) {
            *tempo_acomodacao = tempo - *tempo_variacao_referencia - 0.1;
            flag_acomodacao = true;
        }
    } else {
        if((*var_controle >= *referencia * (1 + (*fator_acomodacao))) || (*var_controle <= *referencia * (1 - (*fator_acomodacao)))) {
            flag_acomodacao = false;
        }
    }
}

void Controle_PID::analise_pico(double tempo) {
    double variacacao_referencia = *referencia - *referencia_passada;
    if(!flag_pico){
        if (variacacao_referencia > 0) {
            if (*var_controle < *var_controle_anterior && *var_controle > *referencia) {
                *tempo_pico = tempo - *tempo_variacao_referencia - 0.2;
                if (!unidade_sobressinal){
                    *sobre_sinal = fabs(100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada));
                } else {
                    *sobre_sinal = fabs(*var_controle_anterior - *referencia);
                }
                flag_pico = true;
            }
        } else {
            if (*var_controle > *var_controle_anterior && *var_controle < *referencia) {
                *tempo_pico = tempo - *tempo_variacao_referencia - 0.2;
                if (!unidade_sobressinal){
                    *sobre_sinal = fabs(100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada));
                } else {
                    *sobre_sinal = fabs(*var_controle_anterior - *referencia);
                }
                flag_pico = true;
            }
        }
    } else {
        if (variacacao_referencia > 0) {
            if (*var_controle > *var_controle_anterior && *var_controle > (unidade_sobressinal ? *referencia + *sobre_sinal : *referencia * (1 + *sobre_sinal))) {
                if (!unidade_sobressinal){
                    *sobre_sinal = 100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada);
                } else {
                    *sobre_sinal = (*var_controle_anterior - *referencia);
                }
                flag_pico = false;
            }
        } else {
            if (*var_controle < *var_controle_anterior && *var_controle < (unidade_sobressinal ? *referencia - *sobre_sinal : *referencia * (1 - *sobre_sinal))) {
                if (!unidade_sobressinal){
                    *sobre_sinal = 100 * (*var_controle_anterior - *referencia)/(*referencia - *referencia_passada);
                } else {
                    *sobre_sinal = (*var_controle_anterior - *referencia);
                }
                flag_pico = false;
            }
        }
    }
}

void Controle_PID::referencia_mudou(double tempo) {
    flag_referencia_mudou = false;
    *tempo_variacao_referencia = tempo - 0.1;
    flag_pico = flag_acomodacao = flag_subida = false;
    flag_subida_inicio = flag_subida_final = false;
}
