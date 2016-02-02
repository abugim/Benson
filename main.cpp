#include <stdio.h>
#include <pthread.h>
#include "classes/tsunami.h"
#include "classes/controle.h"
#include "classes/quanser.h"

void *controle_t(void *param);

int main(int argc, char const *argv[])
{
	printf("Hello World\n");

	// Inicialização da thread de controle
	pthread_t controle;
	pthread_attr_t attr_controle;
	pthread_attr_init(&attr_controle);
	pthread_create(&controle, &attr_controle, controle_t, NULL);
	// Fim inicialização da thread de controle

	while (true) {
		// comunicar com o cliente
	}

	return 0;
}

void *controle_t(void *param)
{
	Quanser* q = new Quanser("10.13.97.69", 20072);

	while(true)
	{
		double read = 0;
		read = q->readAD(4);

		// Calculo do controle

		q->writeDA(4,1);
	}
}
