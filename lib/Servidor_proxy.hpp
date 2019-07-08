#ifndef PROX_SRV_H
#define PROX_SRV_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8228

//classe para para iniciar o servidor:

class Servidor_proxy{

public:
	//realiza a conexão com a porta fornecida(padrão = 8228):
	void start(int);

	//monta o pedido(request) do client :
	std::string monta_request_client();

	//testa a resposta do client:
	void client_reply(std::string);

	//realiza o request:
	std::string realiza_request(std::string);
};
#endif
