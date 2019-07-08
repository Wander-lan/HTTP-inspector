
#include "Servidor_proxy.hpp"
#include "Request.hpp"

int serverFd, socketClient, socketServidor;
struct sockaddr_in endereco;
struct sockaddr_in enderecoServidor;
struct sockaddr    enderecoCliente;
int                enderecoTamanho, valorLido;
int                opt;
char               buffer[64768];


void Servidor_proxy::start(int porta)
{
    if(porta == 0)
    {
        porta = 8228; //>>>> Coloca a porta 8228 como padrao
    }
	enderecoTamanho = sizeof(endereco);
	if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("Ocorreu um erro ao criar o socket");
      exit(EXIT_FAILURE);
  }

  endereco.sin_family      = AF_INET;
  endereco.sin_addr.s_addr = INADDR_ANY;
  endereco.sin_port        = htons(porta);

  if(bind(serverFd, (struct sockaddr *)&endereco,sizeof(endereco)) < 0)
  {
        perror("Ocorreu um erro ao fazer o bind");
        exit(EXIT_FAILURE);
  }
  if(listen(serverFd, 5) < 0)
  {
        perror("Ocorreu um erro ao fazer o listen");
        exit(EXIT_FAILURE);
  }
};


std::string Servidor_proxy::monta_request_client()
{
    using namespace std;
    char buffer[542768];

    bzero((char *) &buffer, sizeof(buffer));

    if((socketClient = accept(serverFd, (struct sockaddr *)&endereco, (socklen_t*)&enderecoTamanho)) < 0)
    {
        perror("Ocorreu um erro ao aceitar conexões");
        exit(EXIT_FAILURE);
    }

    cout << "Conexao aceita!" << endl;
    int num = read(socketClient , buffer, sizeof(buffer));

    if(num > 0)
		{
        string req(buffer);
        Request request = Request(req);
        request.tratar_conexao();
        return request.montar_request();
    }

    Request request = Request();
    request.metodo       = "POST";

    return request.montar_request();
}

std::string Servidor_proxy::realiza_request(std::string req)
{
    using namespace    std;
    struct hostent     *req_host;
    struct sockaddr_in enderecoServidor;
    string             request = req;

    if((socketServidor = socket(AF_INET,SOCK_STREAM,0)) < 0);

    Request reqst = Request(request);
    string host        = reqst.campos["Host:"];
              req_host = gethostbyname(host.c_str());

    if((req_host == NULL) || (req_host->h_addr == NULL))
		{
        std::cout << "Erro ao recuperar informacoes DNS: " <<  host << std::endl;
        exit(1);
    }

    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port  = htons(80);

    memcpy(&enderecoServidor.sin_addr.s_addr,req_host->h_addr,sizeof(req_host->h_addr));

    if(connect(socketServidor,(struct sockaddr *)&enderecoServidor,sizeof(enderecoServidor)) < 0);

    send(socketServidor, request.c_str(), request.length(), 0);

    char buff[1];
    valorLido = read(socketServidor, &buff, sizeof(buff));
    string reply(buff);
    valorLido = read(socketServidor, &buff, sizeof(buff));

		while(valorLido > 0)
		{
        reply.append(buff);
        valorLido = read(socketServidor, &buff, sizeof(buff));  
    }
    return reply;
}

void Servidor_proxy::client_reply(std::string reply)
{
    if(send(socketClient, reply.c_str(), reply.length(), 0) < 0)
		{
        perror("Ocorreu um erro ao responder o browser");
        exit(EXIT_FAILURE);
    }
    close(socketClient);
}
