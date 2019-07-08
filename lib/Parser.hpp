#ifndef HTML_PRS_H
#define HTML_PRS_H
#include <regex>
#include <iostream>
#include <string>
#include <string.h>
#include <list>
#include <set>

//classe responsavel por verificar as informações das mensagens trocadas entre servidor e cliente:

class Parser{

public:
	static std::set<std::string> verifica_emissor(const char*);
	static std::set<std::string> verifica_url   (const char*);
	static std::set<std::string> verifica_import(const char*);
	static std::string verifica_html  (const char*);
};
#endif
