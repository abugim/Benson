#include "controle.h"

Controle::Controle(){
    onda = NULL;

    *nivel_um = 0;
    *nivel_dois = 0;

    *controle = 0;
    *controle_saturado = 0;
}

Controle::~Controle(){
    delete onda;
    delete this;
}

double Controle::acao(){
    *controle = onda->proximo_ponto();
    trava_seguranca();
    return *controle_saturado;
};

char* Controle::reporte(double tempo){
    char* mensagem;
    sprintf(mensagem, "%d\t%lf\t%d\t%lf\t%d\t%lf\t%d\t%lf\t%d\t%lf",
                    TEMPO, tempo,
                    NIVEL_UM, *nivel_um,
                    NIVEL_DOIS, *nivel_dois,
                    T_ONDA, *controle,
                    T_SAT, *controle_saturado);
    return mensagem;
};

void Controle::trava_seguranca(){
    if (*controle > 4) *controle_saturado = 4;
    if (*controle < -4) *controle_saturado = -4;
    if (*nivel_um > 28 && *controle > 3.15) *controle_saturado = 3.15;
    if (*nivel_um > 29 && *controle > 0) *controle_saturado = 0;
    if (*nivel_um < 4 && *controle < 0) *controle_saturado = 0;
};

void Controle::set_nivel_um(double nivel_um){
    *(this->nivel_um) = nivel_um;
};
double Controle::get_nivel_um(){
    return *(this->nivel_um);
};

void Controle::set_nivel_dois(double nivel_dois){
    *(this->nivel_dois) = nivel_dois;
};
double Controle::get_nivel_dois(){
    return *(this->nivel_dois);
};
