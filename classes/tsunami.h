
#include <stdlib.h>
#include <time.h>

#define DEGRAU 0
#define SENOIDAL 1
#define QUADRADA 2
#define SERRA 3
#define ALEATORIO 4

class tsunami
{
public:
	tsunami();
	~tsunami();
	
	double valor() {

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

	// Gets and sets

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