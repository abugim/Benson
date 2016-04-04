#include <stdio.h>
#include <pthread.h>
#include "classes/tsunami.h"
#include "classes/controle.h"
#include "classes/malha_fechada.h"
#include "classes/pid.h"
#include "classes/quanser.h"
#include <arpa/inet.h>
#include <stdlib.h>

#include <math.h>

#include <queue>

#include <sstream>

#include <string>

#include <unistd.h>

// WebSocket
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define CONECTAR 0
#define DESCONECTAR 1
#define CTRL_CONF 2
#define DATA 3
#define ATT_CTRL 4

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

std::queue<char*> fila_msg;

char* ipeu = new char;
int porta;
int leitura_um;
int leitura_dois;
int escrita;
double tempo = 0;

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

void controle_att(stringstream *ss);

void att_pid(stringstream *ss);
void att_pidpid(stringstream *ss);
void att_oe(stringstream *ss);
void att_sr(stringstream *ss);

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
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

	printf("Mensagem: %s\n", msg->get_payload().c_str());

	stringstream ss;
	ss.str(msg->get_payload());
	int tipo_mensagem;
	ss >> tipo_mensagem;
	//printf("Tipo de mensagem: %d\n", tipo_mensagem);

	switch (tipo_mensagem) {
		case CONECTAR:
			conectar(&ss);
		break;

		case DESCONECTAR:
			desconectar();
		break;

		case CTRL_CONF:
			esperando = true;
			tempo = 0;
			while(!fila_msg.empty())
				fila_msg.pop();
			ss >> leitura_um >> leitura_dois >> escrita;
			controle(&ss);
			esperando = false;
		break;

		case ATT_CTRL:
		//string estados;
		esperando = true;
		ss >> leitura_um >> leitura_dois >> escrita;
		if (controle !=  NULL) {
			controle_att(&ss);
			esperando = false;
		}
		break;

		case DATA:
			string estados;
			while (!fila_msg.empty()) {
				//printf("Inserir na fila mensagem: %s\n", fila_msg.front());
				estados += fila_msg.front();
				fila_msg.pop();
				estados += ";";
			}
			s->send(hdl, estados, websocketpp::frame::opcode::text);
		break;
	}
	printf("Recebimento realizado com sucesso\n");
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
			// controlador->acao();
			//printf("Controlador\n");

			/* Write a response to the client */
			estado = controlador->reporte(tempo);
			fila_msg.push(estado);

			tempo += 0.1;
			usleep(100000);
		}
	}
}

void conectar(stringstream *ss){
	*ss >> ipeu >> porta;
	printf("ConexaoParam: %s %d\n", ipeu, porta);
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
	double amp;
	double amp_sup;
	double amp_inf;
	double periodo;
	double periodo_sup;
	double periodo_inf;
	double offset;
	*ss >> tipo_onda >> amp >> amp_sup >> amp_inf
		>> periodo >> periodo_sup >> periodo_inf >> offset;
	onda = new Tsunami(tipo_onda, amp, amp_sup, amp_inf,
						periodo, periodo_sup, periodo_inf, offset);

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
	bool pi_d, unidade_sobressinal, flag_var_controle;
	double kp, ki, kd, fator_subida, fator_acomodacao, talt;
	short int filtro;
	*ss >> fator_subida >> fator_acomodacao >> unidade_sobressinal >> flag_var_controle >> kp >> ki >> kd >> pi_d >> filtro >> talt;
	controlador = new Controle_PID(kp, ki, kd, pi_d, filtro, talt, fator_subida, fator_acomodacao, unidade_sobressinal, flag_var_controle);
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

void controle_att(stringstream *ss){
	set_onda(ss);
	controlador->set_onda(onda);
	int tipo_controle;
	*ss >> tipo_controle;
	switch (tipo_controle) {
		case PID:
			att_pid(ss);
		break;
		case PIDPID:
			att_pidpid(ss);
		break;
		case OE:
			att_oe(ss);
		break;
		case SR:
			att_sr(ss);
		break;
	}
}

void att_pid(stringstream *ss){
	bool pi_d;
	double kp, ki, kd, talt;
	short int filtro;
	*ss >> kp >> ki >> kd >> pi_d >> filtro >> talt;
	double param [6];
	param[0] = kp;
	param[1] = ki;
	param[2] = kd;
	param[3] = pi_d;
	param[4] = filtro;
	param[5] = talt;
	printf("Carregou todos os dados\n");
	controlador->att(param);
}
void att_pidpid(stringstream *ss){
	controlador = new Controle();
}
void att_oe(stringstream *ss){
	controlador = new Controle();
}
void att_sr(stringstream *ss){
	controlador = new Controle();
}
