#ifndef HTTP_RESP_H
#define HTTP_RESP_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

//classe para manipular as mensagens de resposta no servidor proxy:

class Response{

public:
	Response(std::string);
	Response();

	std::string versao;
	std::string dado;
	std::string codigo_status;
	std::map<std::string, std::string> campos;

	//mostra os atributos:
	void printar();
	
	//monta a  mensagem de resposta :
	std::string montar_response();
};
#endif
