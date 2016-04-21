#include "controle_cachoeira.h"

ControleCachoeira::ControleCachoeira(PID *mestre, PID *escravo, Param_Desempenho *param_desempenho)
    : Malha_Fechada(param_desempenho, true) {

    nivel_um_anterior = new double;
    *nivel_um_anterior = 0;
    nivel_dois_anterior = new double;
    *nivel_dois_anterior = 0;
    erro_anterior = new double;
    *erro_anterior = 0;

    erro_escravo = new double;
    *erro_escravo = 0;
    erro_escravo_anterior = new double;
    *erro_escravo_anterior = 0;
    controle_mestre = new double;
    *controle_mestre = 0;

    mestre->set_var(erro);
    if (mestre->pi_d) {
        mestre->set_var_der(erro, erro_anterior);
    } else {
        mestre->set_var_der(nivel_dois, nivel_dois_anterior);
    }
    this->mestre = mestre;

    escravo->set_var(erro_escravo);
    if (escravo->pi_d) {
        escravo->set_var_der(erro_escravo, erro_escravo_anterior);
    } else {
        escravo->set_var_der(nivel_um, nivel_um_anterior);
    }
    this->escravo = escravo;

    erro_escravo = new double;
    erro_escravo = 0;
    controle_mestre = new double;
    controle_mestre = 0;
}

ControleCachoeira::~ControleCachoeira() {
    delete mestre;
    delete escravo;
    delete this;
}

double ControleCachoeira::acao() {
    double referencia_anterior = *referencia;
    *referencia = onda->proximo_ponto();

    flag_referencia_mudou = referencia_anterior != *referencia;
    if (referencia_anterior != *referencia) {
        *referencia_passada = referencia_anterior;
    }

    *erro = *referencia - *nivel_dois;
    mestre->acao(/**controle_mestre == *controle_mestre_saturado*/ false);
    *controle_mestre = *mestre->acao_prop + *mestre->acao_int + *mestre->acao_der;
    *erro_escravo = *controle_mestre - *nivel_um;
    escravo->acao(*controle == *controle_saturado);
    *controle = *escravo->acao_prop + *escravo->acao_int + *escravo->acao_der;

    trava_seguranca();
    return *controle_saturado;
}

char* ControleCachoeira::reporte(double tempo) {
    this->mensagem =  (char*) malloc(2048 * sizeof(char));
    sprintf(mensagem, "%lf|%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf|%lf,%lf,%lf|%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf",
                tempo,*erro, *mestre->acao_prop, *mestre->acao_int, *mestre->acao_der, *controle_mestre,
                *erro_escravo, *escravo->acao_prop, *escravo->acao_int, *escravo->acao_der, *controle,
                *controle_saturado, *nivel_um, *nivel_dois, *referencia,
                flag_pico, *tempo_pico,
                flag_pico, *sobre_sinal,
                flag_subida, *tempo_subida,
                flag_acomodacao, *tempo_acomodacao,
                flag_erro_regime, *erro_regime);
    return mensagem;
}

void ControleCachoeira::att(stringstream *ss) {
    onda->att(ss);
    mestre->att(ss);
    escravo->att(ss);
}
