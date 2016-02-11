#ifndef _CONTROLE_H
#define _CONTROLE_H

#include "tsunami.h"
#include "quanser.h"

#define TEMPO 0
#define NIVEL_UM 1
#define NIVEL_DOIS 2
#define T_ONDA 3
#define T_SAT 4

class Controle
{
public:
	Controle();
	~Controle();

	virtual double acao();
	virtual char* reporte(double tempo);
protected:
	void trava_seguranca(double *sinal_controle, double *nivel_um, double *nivel_dois);

	void set_nivel_um(double nivel_um);
	double get_nivel_um();

	void set_nivel_dois(double nivel_dois);
	double get_nivel_dois();

private:
	Tsunami *onda;

	double *nivel_um;
	double *nivel_dois;

	double *controle_saturado;
	double *controle;
};

#endif
