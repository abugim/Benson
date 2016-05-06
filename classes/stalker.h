#ifndef _SR_H
#define _SR_H

#include "malha_fechada.h"
#include <math.h>

class Stalker : public Malha_Fechada {
public:
    Stalker (Param_Desempenho *param_desempenho, bool flag_var, stringstream *ss);
    virtual ~Stalker ();

    double acao();
    char* reporte(double tempo);
    void att(stringstream *ss);

private:
    double K[3];
    double v_ant;
};

#endif
