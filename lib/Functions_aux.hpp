#ifndef STR_FUN_H
#define STR_FUN_H

#include <iostream>
#include <string>
#include <vector>
#include <string.h>

//classe criada para auxiliar na manipulação de strings:

class Functions_aux{

public:

static std::vector<std::string> split(std::string, const char*);
static std::vector<std::string> splitPrimeiro(std::string, const char*);
static int                      stringParaArquivo(std::string str, const char *caminho,const char *arquivo);
static std::string              stringDoArquivo(const char*arquivo);
static std::string              replace(std::string,const char*,const char*);

};
#endif
