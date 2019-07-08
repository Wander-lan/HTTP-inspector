#include "Response.hpp"
#include "Functions_aux.hpp"
using namespace std;

Response::Response(std::string stringResponse)
{
    vector<string> stringDividida = Functions_aux::split(stringResponse, "\r\n\r\n");

    if(stringDividida.size() < 2)
        stringDividida = Functions_aux::splitPrimeiro(stringResponse, "\n\n");

    string header               = stringDividida[0];
	  dado                        = stringDividida[1];
	  vector<string> hd_lines     = Functions_aux::split(header, "\r\n");
	  string primeiraLinha        = hd_lines[0];
	  vector<string> fl_tokens    = Functions_aux::splitPrimeiro(primeiraLinha, " ");
    versao                      = fl_tokens[0];
    codigo_status                = fl_tokens[1];
    vector<string> temp;

    for(int i = 1; i < hd_lines.size(); i++)
    {
         temp            = Functions_aux::splitPrimeiro(hd_lines[i], " ");
         campos[temp[0]] = temp[1];
    }
};

Response::Response(){};

std::string Response::montar_response()
{
    using  namespace std;
    string Response("");

    Response.append(versao);
    Response.append(" ");
    Response.append(codigo_status);
    Response.append("\r\n");

    campos["Length:"] = to_string(dado.length());

    for (std::map<string,string>::iterator it=campos.begin(); it != campos.end(); ++it)
    {
        Response.append(it->first);
        Response.append(" ");
        Response.append(it->second);
        Response.append("\r\n");
    }
    Response.append("\r\n");
    Response.append(dado);
    Response.append("\r\n");
    return Response;
}


void Response::printar()
{
    cout << "Status: " << codigo_status << endl;
    cout << "Versao: " << versao << endl;

    for(map<string, string>::iterator it = campos.begin(); it != campos.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
    cout << "Dado: " << endl << dado <<endl;
}
