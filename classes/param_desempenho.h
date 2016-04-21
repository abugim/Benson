#ifndef _PARAM_DESEMPENHO_H
#define _PARAM_DESEMPENHO_H

#include <string>
#include <sstream>

using namespace std;

class Param_Desempenho {
public:
    Param_Desempenho(stringstream *ss);
    Param_Desempenho(float fator_subida, float fator_acomodacao, bool unidade_sobressinal);
    virtual ~Param_Desempenho();

    float fator_subida;
    float fator_acomodacao;
    bool unidade_sobressinal;
private:
    int nada;
};

#endif
