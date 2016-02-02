#ifndef _TSUNAMI_H
#define _TSUNAMI_H

#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DEGRAU 0
#define SENOIDAL 1
#define QUADRADA 2
#define SERRA 3
#define ALEATORIO 4

class Tsunami
{
public:
	Tsunami();
	~Tsunami();

<<<<<<< HEAD
	double proximo_ponto();
=======
	double valor();
>>>>>>> master

	// Gets and sets
	void set_tipo(int tipo);
	int get_tipo();

	void set_tempo(double tempo);
	double get_tempo();

	void set_amp(double amp);
	double get_amp();

	void set_amp_sup(double amp_sup);
	double get_amp_sup();

	void set_amp_inf(double amp_inf);
	double get_amp_inf();

	void set_periodo(double periodo);
	double get_periodo();

	void set_periodo_sup(double periodo_sup);
	double get_periodo_sup();

	void set_periodo_inf(double periodo_inf);
	double get_periodo_inf();

	void set_offset(double offset);
	double get_offset();

private:
	int tipo;

	double tempo;

	double amp;
	double amp_sup;
	double amp_inf;

	double periodo;
	double periodo_sup;
	double periodo_inf;

	double offset;
};

#endif
