#ifndef _MF_H
#define _MF_H

#include "controle.h"

#define REF 5
#define ERRO 6
#define ERRO_SAT 7

class Malha_Fechada : public Controle {
public:
    Malha_Fechada ();
    virtual ~Malha_Fechada ();
    double acao();
    char* reporte(double tempo);
private:
    double *referencia;
    double *erro;
};

#endif
