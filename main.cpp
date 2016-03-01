#include <stdio.h>
#include <pthread.h>
#include "classes/tsunami.h"
#include "classes/controle.h"
#include "classes/malha_fechada.h"
#include "classes/pid.h"
#include "classes/quanser.h"
#include <arpa/inet.h>
#include <stdlib.h>

#include <sstream>

#include <string.h>

#include <unistd.h>

// WebSocket
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define CONECTAR 0
#define DESCONECTAR 1
#define CTRL_CONF 3

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;
// --------------------------

bool esperando = true;
Controle* controlador;
Tsunami* onda;
Quanser* q;

char* ipeu = new char;
int porta;
int leitura_um;
int leitura_dois;
int escrita;
double tempo = 0;
websocketpp::connection_hdl *handler;
server *serv = new server();

void* controle_t(void *param);
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

void conectar(stringstream *ss);
void desconectar();

void controle(stringstream *ss);

void set_onda(stringstream *ss);
void set_onda_degrau(stringstream *ss);
void set_onda_senoidal(stringstream *ss);
void set_onda_quadrada(stringstream *ss);
void set_onda_dente_serra(stringstream *ss);
void set_onda_aleatoria(stringstream *ss);

void set_controle(stringstream *ss);
void set_malha_aberta(stringstream *ss);
void set_malha_fechada(stringstream *ss);
void set_pid(stringstream *ss);
void set_pidpid(stringstream *ss);
void set_oe(stringstream *ss);
void set_sr(stringstream *ss);

int main (int argc, char const *argv[]) {
	printf("Inicialização\n");
	// Inicialização da thread de controle
	pthread_t controle;
	pthread_attr_t attr_controle;
	pthread_attr_init(&attr_controle);
	pthread_create(&controle, &attr_controle, controle_t, NULL);
	// Fim inicialização da thread de controle
    try {
        // Set logging settings
        serv->set_access_channels(websocketpp::log::alevel::all);
        serv->clear_access_channels(websocketpp::log::alevel::frame_payload);
        // Initialize Asio
        serv->init_asio();
        // Register our message handler
        serv->set_message_handler(bind(&on_message,serv,::_1,::_2));
        // Listen on port 9002
        serv->listen(9002);
        // Start the serv accept loop
        serv->start_accept();
        // Start the ASIO io_service run loop
        serv->run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
	return 0;
}

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	// recepção e configuração;
	handler = &hdl;

    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }
	printf("Configuração: %s\n", msg->get_payload().c_str());

	stringstream ss;
	ss.str(msg->get_payload());
	int tipo_mensagem;
	ss >> tipo_mensagem;
	switch (tipo_mensagem) {
		case CONECTAR:
			conectar(&ss);
		break;
		case DESCONECTAR:
			desconectar();
		break;
		case CTRL_CONF:
			ss >> leitura_um >> leitura_dois;
			controle(&ss);
			esperando = false;
		break;
	}
}

void *controle_t (void *param) {
	message_ptr msg;

	char* estado;
	printf("Controle inciado\n");
	while(true)
	{
		if (!esperando)
		{
			// Colocar mutex
			controlador->set_nivel_um(q->readAD(leitura_um));
			controlador->set_nivel_dois(q->readAD(leitura_dois));

			// Calculo do controle
			q->writeDA(escrita, controlador->acao());

			/* Write a response to the client */
			estado = controlador->reporte(tempo);
			try {
				serv->send(*handler, estado, websocketpp::frame::opcode::text);
		    } catch (const websocketpp::lib::error_code& e) {
		        std::cout << "Echo failed because: " << e
		                  << "(" << e.message() << ")" << std::endl;
		    }

			tempo += 0.1;
			usleep(100000);
		}
	}
}

void conectar(stringstream *ss){
	*ss >> ipeu >> porta;
	q = new Quanser(ipeu, porta);
}
void desconectar(){
	esperando = true;
	delete q;
}

void controle(stringstream *ss){
	set_onda(ss);
	set_controle(ss);
	controlador->set_onda(onda);
}

void set_onda(stringstream *ss){
	int tipo_onda;
	*ss >> tipo_onda;
	switch (tipo_onda) {
		case DEGRAU:
			set_onda_degrau(ss);
		break;
		case SENOIDAL:
			set_onda_senoidal(ss);
		break;
		case QUADRADA:
			set_onda_quadrada(ss);
		break;
		case SERRA:
			set_onda_dente_serra(ss);
		break;
		case ALEATORIO:
			set_onda_aleatoria(ss);
		break;
	}
}

void set_onda_degrau(stringstream *ss){
	double amp;
	*ss >> amp;
	onda = new Tsunami(SERRA, amp, 0, 0, 0, 0, 0, 0);
}
void set_onda_senoidal(stringstream *ss){
	double amp, periodo, offset;
	*ss >> amp >> periodo >> offset;
	onda = new Tsunami(SENOIDAL, amp, 0, 0, periodo, 0, 0, offset);
}
void set_onda_quadrada(stringstream *ss){
	double amp, periodo, offset;
	*ss >> amp >> periodo >> offset;
	onda = new Tsunami(QUADRADA, amp, 0, 0, periodo, 0, 0, offset);
}
void set_onda_dente_serra(stringstream *ss){
	double amp, periodo, offset;
	*ss >> amp >> periodo >> offset;
	onda = new Tsunami(SERRA, amp, 0, 0, periodo, 0, 0, offset);
}
void set_onda_aleatoria(stringstream *ss){
	double amp_sup, amp_inf, periodo_sup, periodo_inf;
	*ss >> amp_sup >> amp_inf >> periodo_sup >> periodo_inf;
	onda = new Tsunami(ALEATORIO, 0, amp_sup, amp_inf, 0, periodo_sup, periodo_inf, 0);
}

void set_controle(stringstream *ss){
	int tipo_controle;
	*ss >> tipo_controle;
	switch (tipo_controle) {
		case MA:
			set_malha_aberta(ss);
		break;
		case MF:
			set_malha_fechada(ss);
		break;
		case PID:
			set_pid(ss);
		break;
		case PIDPID:
			set_pidpid(ss);
		break;
		case OE:
			set_oe(ss);
		break;
		case SR:
			set_sr(ss);
		break;
	}
}
void set_malha_aberta(stringstream *ss){
	controlador = new Controle();
}
void set_malha_fechada(stringstream *ss){
	controlador = new Malha_Fechada();
}
void set_pid(stringstream *ss){
	bool pi_d;
	double kp, ki, kd;
	*ss >> kp >> ki >> kd >> pi_d;
	controlador = new Controle_PID(kp, ki, kd, pi_d);
}
void set_pidpid(stringstream *ss){
	controlador = new Controle();
}
void set_oe(stringstream *ss){
	controlador = new Controle();
}
void set_sr(stringstream *ss){
	controlador = new Controle();
}
