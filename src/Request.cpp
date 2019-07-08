#include "Request.hpp"
#include "Functions_aux.hpp"

using namespace std;


Request::Request(std::string requestString)
{
    vector<string> requestSeparado = Functions_aux::split(requestString, "\r\n\r\n");
    string         header          = requestSeparado[0];

    if(requestSeparado.size()>1)
        corpo = requestSeparado[1];

    vector<string> linhas        = Functions_aux::split(header, "\r\n");
    string         primeiraLinha = linhas[0];
    vector<string> fl_tokens     = Functions_aux::split(primeiraLinha, " ");

    metodo = fl_tokens[0];
    url    = fl_tokens[1];
    versao = fl_tokens[2];

    vector<string> temp;

    for( int i = 1; i<linhas.size(); i++)
    {
         temp            = Functions_aux::splitPrimeiro(linhas[i], " ");
         campos[temp[0]] = temp[1];
    }
};

Request::Request()
{
    metodo = "GET";
    versao = "HTTP/1.1";
    url    = "";
    corpo  = "";
};

std::string Request::montar_request()
{
    using namespace std;
    string Request("");

    Request.append(metodo);
    Request.append(" ");
    Request.append(url);
    Request.append(" ");
    Request.append(versao);
    Request.append("\r\n");

    for (std::map<string,string>::iterator it=campos.begin(); it!=campos.end(); ++it)
    {
        Request.append(it->first);
        Request.append(" ");
        Request.append(it->second);
        Request.append("\r\n");
    }
    Request.append("\r\n");
    Request.append(corpo);

    return Request;
}

void Request::printar()
{
    cout << "metodo: " << metodo << endl;
    cout << "versao: " << versao << endl;
    cout << "url: " << url << endl;

    for(map<string, string>::iterator it = campos.begin(); it!=campos.end(); ++it)
    {
        cout << it->first << " " << it->second << endl;
    }
    cout << "corpo: " << corpo << endl<<endl;

}

void Request::tratar_conexao()
{
    campos["Connection:"]      = "close";
    campos["Accept-Encoding:"] = "identity";
}

bool Request::avaliar_metodo()
{
    if( metodo != "GET") return false;
    return true;
}
