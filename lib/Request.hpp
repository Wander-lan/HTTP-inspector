#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

//classe para manipular as mensagens de requests no servidor proxy:

class Request{

public:
	Request(std::string);
	Request();

	std::map<std::string, std::string> campos;
	std::string corpo;
	std::string metodo;
	std::string url;
	std::string versao;

	//verifica se o metodo é GET:
	bool        avaliar_metodo();

	//acrescenta "close" e "identity" nos campos corretos da mensagem:
	void        tratar_conexao();

	//mostra os atributos:
	void        printar();

	//monta o request:
	std::string montar_request();
};
#endif
