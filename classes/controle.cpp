#include "controle.h"

double Controle::acao(){
  return this->referencia->valor();
}

char* Controle::reporte(){
  // Implementar
  return char*;
}

void Controle::trava_seguranca(double *sinal_controle, double *nivel_um, double *nivel_dois){
  if (*sinal_controle > 4) *sinal_controle = 4;
  if (*sinal_controle < -4) *sinal_controle = -4;
  if (*nivel_um > 28 && *sinal_controle > 3.15) *sinal_controle = 3.15;
  if (*nivel_um > 29 && *sinal_controle > 0) *sinal_controle = 0;
  if (*nivel_um < 4 && *sinal_controle < 0) *sinal_controle = 0;
}
