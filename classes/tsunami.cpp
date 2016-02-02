#include "tsunami.h"

Tsunami::Tsunami(){;}

Tsunami::~Tsunami(){;}

double Tsunami::proximo_ponto() {
  if (tempo > periodo){
    tempo = 0;
  } else {
    tempo += 0.1;
  }

  switch(tipo)
  {
    case DEGRAU:
      return this->amp + this->offset;
      break;

    case SENOIDAL:
      return amp * sin((tempo / periodo) * 2 * M_PI) + this->offset;
      break;

    case QUADRADA:
      if (tempo < periodo/2.f)
        return this->amp + this->offset;
      else
        return -this->amp + this->offset;
      break;

    case SERRA:
      return (2 * amp / periodo) * tempo - amp + this->offset;
      break;

    case ALEATORIO:
      if (tempo == 0) {
        srand (time(NULL));
        periodo = ((float) (rand()) / (float) (RAND_MAX)) * (periodo_sup - periodo_inf) + periodo_inf;
        amp = ((float) (rand()) / (float) (RAND_MAX)) * (amp_sup - amp_inf) + amp_inf;
      }
      return this->amp;
      break;
  }
}

void Tsunami::set_tipo(int tipo){
  this->tipo = tipo;
}
int Tsunami::get_tipo(){
  return this->tipo;
}

void Tsunami::set_tempo(double tempo){
  this->tempo = tempo;
}
double Tsunami::get_tempo(){
  return this->tempo;
}

void Tsunami::set_amp(double amp){
  this->amp = amp;
}
double Tsunami::get_amp(){
  return this->amp;
}

void Tsunami::set_amp_sup(double amp_sup){
  this->amp_sup = amp_sup;
}
double Tsunami::get_amp_sup(){
  return this->amp_sup;
}

void Tsunami::set_amp_inf(double amp_inf){
  this->amp_inf = amp_inf;
}
double Tsunami::get_amp_inf(){
  return this->amp_inf;
}

void Tsunami::set_periodo(double periodo){
  this->periodo = periodo;
}
double Tsunami::get_periodo(){
  return this->periodo;
}

void Tsunami::set_periodo_sup(double periodo_sup){
  this->periodo_sup = periodo_sup;
}
double Tsunami::get_periodo_sup(){
  return this->periodo_sup;
}

void Tsunami::set_periodo_inf(double periodo_inf){
  this->periodo_inf = periodo_inf;
}
double Tsunami::get_periodo_inf(){
  return this->periodo_inf;
}

void Tsunami::set_offset(double offset){
  this->offset = offset;
}
double Tsunami::get_offset(){
  return this->offset;
}
