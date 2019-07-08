#include "proxyui.h"
#include "ui_proxyui.h"
#include "Servidor_proxy.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Functions_aux.hpp"
#include "Spider.hpp"


using namespace std;
proxyui::proxyui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::proxyui)
{
    ui->setupUi(this);
}

proxyui::~proxyui()
{
    delete ui;
}
void proxyui::startInsp(int porta)
{
    server = Servidor_proxy();

    server.start(porta);
        req = server.monta_request_client();
        ui->te_request->setPlainText(req.c_str());
        Request request = Request(req);

            Functions_aux::stringParaArquivo(req, ".", "request.txt");
            req                    = Functions_aux::stringDoArquivo("request.txt");
}

void proxyui::spider(char* url)
{
    Spider spider = Spider(url);
    if(spider.valido)
    {
        string aux;
        spider.montar_arvore(2);
        aux = " ";
        aux += spider.root ;

        int numSpacos     = spider.root.size()+5;
        set<string> filhos = spider.par[spider.root];
        set<string> visitado;

        visitado.insert(spider.root);

        for(set<string>::iterator it = filhos.begin(); it != filhos.end(); ++it){
            if(visitado.find(*it) == visitado.end())
            {
                visitado.insert(*it);
                aux+= *it +"---> ";

                int numSpacos2        = numSpacos + (*it).size()+5;
                set<string> granfilhos = spider.par[*it];

                for(set<string>::iterator it2 = granfilhos.begin(); it2!=granfilhos.end(); ++it2)
                {
                    aux += *it2 + "\n";
                    for(int k =0; k<numSpacos2; k++)
                        aux =+ " ";
                }
            }
            aux += "\n";
            for(int k =0; k < numSpacos; k++) aux+= " ";
        }
        aux+= "\n";
        ui->te_spider->setPlainText(aux.c_str());
    }
}

void proxyui::on_pb_spidergo_clicked()
{
    QByteArray ba = ui->le_spider->text().toLocal8Bit();
    char *c_str = (char*)ba.data();
    spider(c_str);
}

void proxyui::on_pb_start_clicked()
{
    startInsp(ui->le_port->text().toInt());
}

void proxyui::on_pb_dump_clicked()
{
    QByteArray ba = ui->le_dump->text().toLocal8Bit();
    char *c_str = (char*)ba.data();
    Spider x = Spider(c_str);
    x.dump(2);
}

void proxyui::on_pb_request_clicked()
{
    string           req = server.monta_request_client();
    ui->te_request->setPlainText(req.c_str());
    Request request = Request(req);
}

void proxyui::on_pb_response_clicked()
{
    reply           = server.realiza_request(req);
    Response response = Response(reply);

    Functions_aux::stringParaArquivo(reply, ".", "response.txt");
    reply = Functions_aux::stringDoArquivo("response.txt");
    ui->te_response->setPlainText(reply.c_str());
    server.client_reply(reply);
}
