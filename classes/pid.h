#ifndef _PID_H
#define _PID_H

#include "malha_fechada.h"

#define ACAOP 8
#define ACAOI 9
#define ACAOD 10
#define CONTROLE 11
#define CONTROLE_SATURADO 12

#define SEM__FILTRO 0
#define BACK_FILTRO 1
#define COND_FILTRO 2

class Controle_PID : public Malha_Fechada {
public:
    Controle_PID(double kp, double ki, double kd, bool pi_d, short int filtro, double talt, float fator_subida, float fator_acomodacao, bool unidadeSobressinal, bool var_controle);
    virtual ~Controle_PID();
    double acao();
    char* reporte(double tempo);

    void att(double param[]);
    double acaoP();
    double acaoI();
    double acaoD();

    void back();

    void analise_desempenho(double tempo);

private:
    // Flag para determinar se é PID ou PI-D
    double *var_controle;
    double *var_controle_anterior;
    bool pi_d;

    double *integrador;
    double *erro_anterior;
    double *nivel_um_anterior;

    double *kp;
    double *ki;
    double *kd;

    bool flag_saturado;
    double *talt;

    double *acao_prop;
    double *acao_int;
    double *acao_der;

    short int filtro;

    bool flag_referencia_mudou;

    double *referencia_passada;
    double *tempo_variacao_referencia;

    bool unidade_sobressinal;
    double *fator_subida;
    double *fator_acomodacao;

    double *tempo_subida;
    double *tempo_subida_inicio;
    double *tempo_subida_final;

    double *tempo_acomodacao;
    double *tempo_pico;
    double *sobre_sinal;

    bool flag_subida;
    bool flag_subida_inicio;
    bool flag_subida_final;

    bool flag_acomodacao;

    bool flag_pico;

    void referencia_mudou(double tempo);
    void analise_tempo_subida(double tempo);
    void analise_tempo_acomodacao(double tempo);
    void analise_pico(double tempo);
};

#endif
