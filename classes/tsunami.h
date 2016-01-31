
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
private:
	double valor();
	
	double tempo;

	double amp;
	double amp_sup;
	double amp_inf;

	double period;
	double period_sup;
	double period_inf;

	double offset;
};