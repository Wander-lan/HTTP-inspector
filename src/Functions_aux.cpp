#include "Functions_aux.hpp"
using namespace std;

std::vector<std::string> Functions_aux::split(std::string str, const char * delimitador)
{
    vector<string> tokens;
    string os(delimitador);
    int offset = os.length();

    std::size_t ultimo = -1*offset;
    std::size_t achado = str.find(delimitador, ultimo+offset);
    while(achado < str.length())
    {
        tokens.push_back(str.substr(ultimo+offset, achado - ultimo -offset));
        ultimo = achado;
        achado = str.find(delimitador, ultimo+offset);
    }
    if((ultimo+offset) < str.length())
        tokens.push_back(str.substr(ultimo+offset, str.length()));

    return tokens;
}

std::vector<std::string> Functions_aux::splitPrimeiro(std::string str, const char * delimitador)
{
    vector<string> tokens;
    string os(delimitador);
    int offset = os.length();

    std::size_t achado = str.find(delimitador);

    if(achado > (str.length()-offset)){
        tokens.push_back(str);
        tokens.push_back(str);
        return tokens;
    }

    tokens.push_back(str.substr(0, achado));
    tokens.push_back(str.substr(achado+offset, str.length()-offset));

    return tokens;
}

int Functions_aux::stringParaArquivo(std::string str, const char *path, const char *file)
{

    FILE *f;
    string caminhoCompletoCompleto("");
    caminhoCompletoCompleto.append(path);
    caminhoCompletoCompleto.append("/");
    caminhoCompletoCompleto.append(file);
    f = fopen(caminhoCompletoCompleto.c_str(), "w");
    if(f==NULL)
    {
        printf("ERRO NO PONTEIRO");
        exit(2);
    }
    //fwrite( str.data(),str.length(),1, f);
  //  printf("%s",str.c_str());
    fprintf(f,"%s",str.c_str());
    fclose(f);

    return 0;
}

std::string Functions_aux::stringDoArquivo(const char*file)
{

    FILE *f;
    char buffer[64768];
    f = fopen(file, "rb");
    fread (buffer, 1, sizeof(buffer), f);
    fclose(f);

    std::string res(buffer);
    return res;
}

std::string Functions_aux::replace(std::string file, const char*from,const char*to)
{

    string str = file;
    string os(from);
    int size   = os.length();
    int achado = str.find(from);
    while(achado < str.length())
    {
        str.replace(achado, size, to);
        achado = str.find(from);
    }

    return str;
}
