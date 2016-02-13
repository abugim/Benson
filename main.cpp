#include <stdio.h>
#include <pthread.h>
#include "classes/tsunami.h"
#include "classes/controle.h"
#include "classes/malha_fechada.h"
#include "classes/quanser.h"

#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

bool esperando = true;
Controle* controlador;
Quanser* q;

char* ipeu;
int porta;
int leitura_um;
int leitura_dois;
int escrita;
int newsockfd;
bool debug = false;
bool debug_local = false;
bool sem_planta = false;

void* controle_t(void *param);

int main(int argc, char const *argv[])
{
	if (argc >= 2)
		sem_planta = atoi(argv[1]) == 1 ? true : false;

	printf("Inicialização\n");
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 54321;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
    	exit(1);
    }

     /* Now start listening for the clients, here process will
        * go in sleep mode and will wait for the incoming connection
     */

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    printf("Esperando Cliente\n");
    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    printf("Cliente recebido\n");

    if (newsockfd < 0) {
		perror("ERROR on accept");
		exit(1);
    }

	// recepção e configuração;
	if (!sem_planta) {
		bzero(buffer,256);
		n = recv(newsockfd, buffer, 255, 0);
		printf("Configuração recebida.\n");
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}

		sscanf(buffer, "%s %d %d %d %d", ipeu, &porta, &leitura_um, &leitura_dois, &escrita);
		q = new Quanser(ipeu, porta);
	}
	// q = new Quanser("10.13.99.69", 20081);
	//
	// leitura_um = 0;
	// leitura_dois = 1;
	// escrita = 0;

	// Inicialização da thread de controle
	pthread_t controle;
	pthread_attr_t attr_controle;
	pthread_attr_init(&attr_controle);
	pthread_create(&controle, &attr_controle, controle_t, NULL);
	// Fim inicialização da thread de controle

	while (true) {
		if (!sem_planta){
			// recepção e configuração;
			bzero(buffer,256);
		    	n = recv(newsockfd, buffer, 255, 0);
		    printf("Configuração recebida.\n");
		    if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
		    }
		    printf("Configuração: %s\n", buffer);

			int tipo;
			double amp, amp_sup, amp_inf, periodo, periodo_sup, periodo_inf, offset;
			sscanf(buffer, "%d %lf %lf %lf %lf %lf %lf %lf",
							&tipo, &amp, &amp_sup, &amp_inf, &periodo, &periodo_sup, &periodo_inf, &offset);
			//Tsunami *onda = new Tsunami(tipo, amp, amp_sup, amp_inf, periodo, periodo_sup, periodo_inf, offset);
			sscanf(buffer, "%d", &tipo);
			// Colocar mutex
			switch (tipo) {
					case PIDPID:
					break;
				case PID:
					break;
				case OE:
					break;
				case SR:
					break;
				case MA:
					controlador = new Controle();
					break;
				case MF:
					controlador = new Malha_Fechada();
					break;
				default:
					break;
			}
		}
		esperando = false;
	}
	return 0;
}

void *controle_t(void *param)
{
	char* estado;
	double tempo = 0;
	int n = 0;

	printf("Controle inciado\n");
	controlador = new Malha_Fechada();
	controlador->set_onda(new Tsunami(DEGRAU, 15, 0, 0, 0, 0, 0, 0));

	Tsunami *nivel_um_fake = new Tsunami(QUADRADA, 5, 0, 0, 5, 0, 0, 10);
	Tsunami *nivel_dois_fake = new Tsunami(SERRA, 5, 0, 0, 5, 0, 0, 10);
	while(true)
	{
		if (!esperando)
		{
			// Colocar mutex
			// controlador->set_nivel_um(q->readAD(leitura_um));
			// controlador->set_nivel_dois(q->readAD(leitura_dois));
			controlador->set_nivel_um(nivel_um_fake->proximo_ponto());
			controlador->set_nivel_dois(nivel_dois_fake->proximo_ponto());

			// Calculo do controle
			//q->writeDA(escrita, controlador->acao());
			controlador->acao();

			/* Write a response to the client */
			estado = controlador->reporte(tempo);
		    n = write(newsockfd, estado, strlen(estado));
		    if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
		    }
			printf("Estado enviado\n");

			tempo += 0.1;
			usleep(100000);
		}
	}
}
