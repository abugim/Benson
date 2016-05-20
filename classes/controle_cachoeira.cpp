#include "controle_cachoeira.h"

ControleCachoeira::ControleCachoeira(Param_Desempenho *param_desempenho, PID *mestre, PID *escravo)
    : Malha_Fechada(param_desempenho, true) {

    nivel_um_anterior = new double(0);
    nivel_dois_anterior = new double(0);

    erro_escravo = new double(0);
    erro_escravo_anterior = new double(0);
    controle_mestre = new double(0);

    var_controle = nivel_dois;
    var_controle_anterior = nivel_dois_anterior;

    this->mestre = mestre;
    this->escravo = escravo;

    mestre->set_var(erro, erro_anterior);
    if (mestre->pi_d) {
        mestre->set_var_der(nivel_dois, nivel_dois_anterior);
    } else {
        mestre->set_var_der(erro, erro_anterior);
    }

    escravo->set_var(erro_escravo, erro_escravo_anterior);
    if (escravo->pi_d) {
        escravo->set_var_der(nivel_um, nivel_um_anterior);
    } else {
        escravo->set_var_der(erro_escravo, erro_escravo_anterior);
    }

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

    *erro_anterior = *erro;
    *erro_escravo_anterior = *erro_escravo;
    *nivel_um_anterior = *nivel_um;
    *nivel_dois_anterior = *nivel_dois;

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
