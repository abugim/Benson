#ifndef _MF_H
#define _MF_H

#include "param_desempenho.h"
#include "controle.h"

#define REF 5
#define ERRO 6
#define ERRO_SAT 7

class Malha_Fechada : public Controle {
public:
    Malha_Fechada (Param_Desempenho *param_desempenho, bool flag_var);
    virtual ~Malha_Fechada ();

    double acao();
    char* reporte(double tempo);
    void set_var_control(double *var_controle);
    void att(stringstream *ss);

protected:
    double *var_controle;
    double *var_controle_anterior;

    double *referencia;
    double *referencia_passada;

    double *erro;

    /* Analise de Desempenho */
    Param_Desempenho *param_desempenho;

    void analise_desempenho(double tempo);
    void referencia_mudou(double tempo);
    void analise_tempo_subida(double tempo);
    void analise_tempo_acomodacao(double tempo);
    void analise_pico(double tempo);
    void analise_erro();

    bool flag_referencia_mudou;
    double *tempo_variacao_referencia;

    bool flag_subida;
    bool flag_subida_inicio;
    bool flag_subida_final;
    double *tempo_subida;
    double *tempo_subida_inicio;
    double *tempo_subida_final;

    bool flag_acomodacao;
    double *tempo_acomodacao;

    bool flag_pico;
    double *tempo_pico;
    double *sobre_sinal;

    bool flag_erro_regime;
    double *erro_regime;
    /* Analise Desempenho */
};

#endif
