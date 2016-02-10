#include <stdio.h>
#include <pthread.h>
#include "classes/tsunami.h"
#include "classes/controle.h"
#include "classes/quanser.h"

#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

bool esperando = true;

void *controle_t(void *param);

int main(int argc, char const *argv[])
{
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

    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }

	// Inicialização da thread de recepção
	pthread_t receptor;
	pthread_attr_t attr_receptor;
	pthread_attr_init(&attr_receptor);
	pthread_create(&receptor, &attr_receptor, receptor_t, NULL);
	// Fim inicialização da thread de recepção

	// Inicialização da thread de controle
	pthread_t controle;
	pthread_attr_t attr_controle;
	pthread_attr_init(&attr_controle);
	pthread_create(&controle, &attr_controle, controle_t, NULL);
	// Fim inicialização da thread de controle

	while (true) {
		// recepção e configuração;
		bzero(buffer,256);
	    n = recv(newsockfd, buffer, 255, 0);
	    printf("received\n");
	    if (n < 0) {
	      perror("ERROR reading from socket");
	      exit(1);
	    }
		// Compartilha mensagem com thread de controle
	    printf("Here is the message: %s\n",buffer);

		esperando = false;
	}

	return 0;
}

void *controle_t(void *param)
{
	Quanser* q = new Quanser("10.13.97.69", 20072);

	while(true)
	{
		if (!esperando)
		{
			controlador.set_nivel_um(q->readAD(leitura_um));
			controlador.set_nivel_dois(q->readAD(leitura_dois));

			// Calculo do controle
			q->writeDA(escrita, controlador.acao());

			/* Write a response to the client */
		    n = send(newsockfd,"I got your message", 18, 0);
		    printf("sent\n");
		    if (n < 0) {
		      perror("ERROR writing to socket");
		      exit(1);
		    }
		}
	}
}
