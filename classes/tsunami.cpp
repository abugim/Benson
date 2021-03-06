#include "tsunami.h"
#include <stdio.h>

Tsunami::Tsunami(){
    this->tempo = 0;
    this->tipo = 0;
    this->amp = 0;
    this->amp_sup = 0;
    this->amp_inf = 0;
    this->periodo = 0;
    this->periodo_sup = 0;
    this->periodo_inf = 0;
    this->offset = 0;
}

Tsunami::Tsunami(stringstream *ss) {
    tempo = 0;
    *ss >> tipo >> amp >> amp_sup >> amp_inf
		>> periodo >> periodo_sup >> periodo_inf >> offset;
}

Tsunami::Tsunami(int tipo, double amp, double amp_sup, double amp_inf, double periodo, double periodo_sup, double periodo_inf, double offset){
    this->tempo = 0;
    this->tipo = tipo;
    this->amp = amp;
    this->amp_sup = amp_sup;
    this->amp_inf = amp_inf;
    this->periodo = periodo;
    this->periodo_sup = periodo_sup;
    this->periodo_inf = periodo_inf;
    this->offset = offset;
}

Tsunami::~Tsunami(){
    delete this;
}

void Tsunami::att(stringstream *ss) {
    *ss >> tipo >> amp >> amp_sup >> amp_inf
		>> periodo >> periodo_sup >> periodo_inf >> offset;
}

double Tsunami::proximo_ponto() {
    if (this->tempo + 0.1 > this->periodo){
        this->tempo = 0;
    } else {
        this->tempo += 0.1;
    }

    switch(tipo)
    {
        case DEGRAU:
        return this->amp + this->offset;
        break;

        case SENOIDAL:
        return this->amp * sin((this->tempo / this->periodo) * 2.f * M_PI) + this->offset;
        break;

        case QUADRADA:
        if (this->tempo < this->periodo/2.f)
        return this->amp + this->offset;
        else
        return -this->amp + this->offset;
        break;

        case SERRA:
        return (2.f * this->amp / this->periodo) * this->tempo - this->amp + this->offset;
        break;

        case ALEATORIO:
        if (this->tempo == 0) {
            srand(time(NULL));
            this->periodo = (((float) (rand()) / (float) (RAND_MAX)) * (this->periodo_sup - this->periodo_inf)) + this->periodo_inf;
            printf("p: %lf\n", this->periodo);
            this->amp = (((float) (rand()) / (float) (RAND_MAX)) * (this->amp_sup - this->amp_inf)) + this->amp_inf;
            printf("a: %lf\n", this->amp);

        }
        return this->amp;
        break;

        default:
        return 0;
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
