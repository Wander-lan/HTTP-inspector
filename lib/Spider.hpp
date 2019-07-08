#ifndef SPIDER_H
#define SPIDER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <set>

using namespace std;


//classe para realizar o spider:

class Spider{

public:
	Spider(std::string);

	std::string host; 
	std::string root;
	bool valido;							
	std::map<std::string, std::string> nomes;
	std::map<std::string, std::string> htmls;
    	std::set<std::string> urls_visitadas;
	std::map<std::string, std::string> dicionario;
        std::map<std::string, std::set<std::string> > par;

	bool        avaliar_url(std::string url);

	std::string parse_url(std::string url);
	std::string url_nome_arquivo(std::string url);

	//monta a arvore de referencias:
	void        montar_arvore(int);

	//mostra a arvore:
	void        printar_arvore(int);

	//realiza o dump:
	void        dump(int);
};
#endif
