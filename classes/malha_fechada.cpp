#include "malha_fechada.h"

Malha_Fechada::Malha_Fechada(){

}

Malha_Fechada::~Malha_Fechada(){

}

double Controle::acao(){
  return *(this->referencia) - *(this->nivel_um);
}

char* Controle::reporte(){

}
