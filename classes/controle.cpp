#include "controle.h"

Controle::Controle(){
    this->nivel_um = new double;
    this->nivel_dois = new double;

    this->controle = new double;
    this->controle_saturado = new double;
    this->mensagem =  (char*) malloc(2048 * sizeof(char));
}

Controle::~Controle(){
    delete onda;
    delete this;
}

void Controle::att(double param[]){}

double Controle::acao(){
    *(this->controle) = this->onda->proximo_ponto();
    trava_seguranca();
    return *(this->controle_saturado);
};

char* Controle::reporte(double tempo){
    this->mensagem =  (char*) malloc(2048 * sizeof(char));
    sprintf(mensagem, "%lf|%lf,%lf|%lf,%lf",
                tempo, *controle, *controle_saturado, *nivel_um, *nivel_dois);
    return mensagem;
};

void Controle::trava_seguranca(){
    *(this->controle_saturado) = *(this->controle);
    if (*(this->controle) > 4) *(this->controle_saturado) = 4;
    if (*(this->controle) < -4) *(this->controle_saturado) = -4;
    if (*(this->nivel_um) > 28 && *(this->controle) > 3.15) *(this->controle_saturado) = 3.15;
    if (*(this->nivel_um) > 29 && *(this->controle) > 0) *(this->controle_saturado) = 0;
    if (*(this->nivel_um) < 4 && *(this->controle) < 0) *(this->controle_saturado) = 0;
};


void Controle::set_onda(Tsunami* onda){
    this->onda = onda;
};

void Controle::set_nivel_um(double nivel_um){
    *(this->nivel_um) = nivel_um * CONSTANTE_SENSOR;
};
double Controle::get_nivel_um(){
    return *(this->nivel_um);
};

void Controle::set_nivel_dois(double nivel_dois){
    *(this->nivel_dois) = nivel_dois * CONSTANTE_SENSOR;
};
double Controle::get_nivel_dois(){
    return *(this->nivel_dois);
};
