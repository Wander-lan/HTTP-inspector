#include "Parser.hpp"
#include "Spider.hpp"
#include "Servidor_proxy.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Functions_aux.hpp"
#include "proxyui.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    int porta = 8228;
        if(argc == 3 and (std::string)argv[1] == "-p")
        {
            porta = atoi(argv[2]);
            if(porta < 0 or porta > 100000)
            {
                exit(1);
            }
        }

    QApplication a(argc, argv);
    proxyui w;
    w.show();

    return a.exec();
}
