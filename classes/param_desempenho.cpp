#include "param_desempenho.h"

Param_Desempenho::Param_Desempenho(stringstream *ss) {
    *ss >> fator_subida >> fator_acomodacao >> unidade_sobressinal;
}

Param_Desempenho::Param_Desempenho(float fator_subida, float fator_acomodacao, bool unidade_sobressinal) {
    this->fator_subida = fator_subida;
    this->fator_acomodacao = fator_acomodacao;
    this->unidade_sobressinal = unidade_sobressinal;
}

Param_Desempenho::~Param_Desempenho() {

}
