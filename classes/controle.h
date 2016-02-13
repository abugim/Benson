#ifndef _CONTROLE_H
#define _CONTROLE_H

#include "tsunami.h"
#include "quanser.h"

#define TEMPO 0
#define NIVEL_UM 1
#define NIVEL_DOIS 2
#define T_ONDA 3
#define T_SAT 4

#define MA 0
#define MF 1
#define PID 2
#define PIDPID 3
#define OE 4
#define SR 5

#define CONSTANTE_SENSOR 6.25

class Controle
{
public:
	Controle();
	virtual ~Controle();

	virtual double acao();
	virtual char* reporte(double tempo);

	void set_onda(Tsunami* onda);

	void set_nivel_um(double nivel_um);
	double get_nivel_um();

	void set_nivel_dois(double nivel_dois);
	double get_nivel_dois();

protected:
	void trava_seguranca();
	Tsunami *onda;

	double *nivel_um;
	double *nivel_dois;

	double *controle_saturado;
	double *controle;
};

#endif
