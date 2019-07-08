#include "Parser.hpp"
#include "Spider.hpp"
#include "Servidor_proxy.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Functions_aux.hpp"
using namespace std;


Spider::Spider(string url)
{
	string internal       = url;
	vector<string> result = Functions_aux::split(internal, "//");

	if(result.size() == 2)
	{
		if(result[0] != "http:")
		{
		 valido = false;
		 return;
		}

		internal = result[1];
	}
	if(result.size() > 2)
	{
	 	valido = false;
	 	return;
	}

	if(internal.find('/')!=0 && internal.find('/') < internal.length())
	{
		vector<string> result2 = Functions_aux::splitPrimeiro(internal, "/");
		if(host.length() == 0)
		{
			host = result2[0];
		}else
		{
			valido = false;
			return;
		}
		root = "/";
		root.append(result2[1]);
	}else if(internal.find('/') > internal.length())
	{
		root = "/";
		host = internal;
	}else
	{
		valido = false;
		return;
	}
	valido = true;
};

bool Spider::avaliar_url(string url)
{
	if(url.size() == 0)
		return false;

	string internal       = url;
	vector<string> result = Functions_aux::split(internal, "//");

	if(result.size() == 2)
	{
		if(result[0] != "http:")
			return false;
		internal = result[1];
	}

	if(result.size() > 2)
		return false;


	if(internal.find('/') != 0)
	{
		vector<string> result2 = Functions_aux::split(internal, "/");

		if(host.length() == 0)
		{
			return false;
		}else
		{
			if(host != result2[0])
				return false;
		}
	}
	return true;
}

string Spider::parse_url(string url)
{
	string parsed("");
	string internal       = url;
	vector<string> result = Functions_aux::split(internal, "//");

	if(result.size() == 2)
		internal = result[1];

	if(internal.find('/') != 0)
	{
		vector<string> result2 = Functions_aux::splitPrimeiro(internal, "/");
		parsed                 = "/";
		parsed.append(result2[1]);
	}else
	{
		parsed = internal;
	}

	if(parsed.find("?") < parsed.length())
	{
		int ask = parsed.find("?");
		parsed  = parsed.substr(0,ask);
	}

	if(parsed != url)
		nomes[parsed] = url;

	return parsed;
}

void Spider::montar_arvore(int niveis)
{
	int i = niveis;
	urls_visitadas.clear();
	par.clear();
	htmls.clear();

	if(!valido)
		return;


	Servidor_proxy proxy   = Servidor_proxy();
	Request request = Request();
	request.tratar_conexao();
	set<string> requestTo;
	requestTo.insert(root);

	do
	{
		set<string> nextRequest;

		for (std::set<string>::iterator url=requestTo.begin(); url != requestTo.end(); ++url)
		{
			if(urls_visitadas.find(*url) == urls_visitadas.end())
			{
				urls_visitadas.insert(*url);
				request.url             = *url;
				request.campos["Host:"] = host;


				string reply           = proxy.realiza_request(request.montar_request());
				Response response = Response(reply);

				if(response.codigo_status == "200 OK")
				{
					htmls[*url]           = response.dado;
					set<string> todosUrl = Parser::verifica_url(response.dado.c_str());
					set<string> urlLocal;

					for (std::set<string>::iterator it = todosUrl.begin(); it != todosUrl.end(); ++it)
					{
				        if(this->avaliar_url(*it))
				        	urlLocal.insert(this->parse_url(*it));

		     		}
				    par[*url] = urlLocal;
				    for (std::set<string>::iterator it=urlLocal.begin(); it != urlLocal.end(); ++it)
				        nextRequest.insert(*it);
				}
			}
		}
		requestTo = nextRequest;
	 	i--;
	}while(i>=0);
}

std::string Spider::url_nome_arquivo(std::string url_in)
{

	string nomeArquivo("");
	string url = url_in;
	string slash("-");
	vector<string> splitted = Functions_aux::split(url, "/");
	if(splitted.size()==1 && splitted[0].length()==0)
	{
		nomeArquivo = "root.html";
		return nomeArquivo;
	}
	for(int i = 0; i < splitted.size(); i++)
			nomeArquivo.append(splitted[i]);

	if(nomeArquivo.find(".") > nomeArquivo.length())
		nomeArquivo.append(".html");
	else if(nomeArquivo.find("?") < nomeArquivo.length()){
		int ask  = nomeArquivo.find("?");
		nomeArquivo = nomeArquivo.substr(0,ask);
	}
	return nomeArquivo;
}

void Spider::dump(int niveis)
{
	this->montar_arvore(niveis);
	set<string> traduzir = urls_visitadas;

	dicionario[root] = "index.html";
	traduzir.erase(root);

	for(set<string>::iterator it = traduzir.begin(); it != traduzir.end(); ++it)
		dicionario[*it]=Spider::url_nome_arquivo(*it);

	for(std::map<string,string>::iterator it = htmls.begin(); it != htmls.end(); ++it)
    	for(std::map<string,string>::iterator dt = dicionario.begin(); dt != dicionario.end(); ++dt)
			{
    		it->second = Functions_aux::replace(it->second,"\'","\"");
    		string from("\"");from.append(dt->first.c_str());from.append("\"");
    		string to("\"");to.append(dt->second.c_str());to.append("\"");
    		it->second = Functions_aux::replace(it->second,from.c_str(),to.c_str());

    		if(nomes.find(dt->second) != nomes.end())
				{
    			string from("\"");from.append(nomes[dt->second].c_str());from.append("\"");
    			string to("\"");to.append(dt->second.c_str());to.append("\"");
    			it->second = Functions_aux::replace(it->second,from.c_str(),to.c_str());
    		}
    }

  string command("rm -rf ");
  command.append(host);
	system(command.c_str());
  command.assign("mkdir ");
  command.append(host);
	system(command.c_str());

   for(std::map<string,string>::iterator it = htmls.begin(); it != htmls.end(); ++it)
	{
		cout << "Salvando arquivo: " << dicionario[it->first] << endl;
    Functions_aux::stringParaArquivo(it->second, host.c_str(), dicionario[it->first].c_str());
  }
  cout << "Arquivos salvos no diretorio " << host << endl;
  cout << "O arquivo \"index.html\" eh a raiz." << endl;
}

void Spider::printar_arvore(int niveis)
{

	if(niveis==0)
	{
		cout << "Arvore de urls:"<<endl<<endl;
		cout << root << "---> ";

		int numSpacos     = root.size()+5;
		set<string> filhos = par[root];
		for(set<string>::iterator it = filhos.begin(); it != filhos.end(); ++it)
		{
			cout << *it << endl;
			for(int s =0; s < numSpacos; s++)
				cout << " ";

		}
		cout << endl;
	}
	else if(niveis == 1)
	{
		cout << "Arvore de urls:"<<endl<<endl;
		cout << root << "---> ";

		int numSpacos     = root.size()+5;
		set<string> filhos = par[root];
		set<string> visitado;

		visitado.insert(root);

		for(set<string>::iterator it = filhos.begin(); it != filhos.end(); ++it){
			if(visitado.find(*it) == visitado.end())
			{
				visitado.insert(*it);
				cout << *it << "---> ";

				int numSpacos2        = numSpacos + (*it).size()+5;
				set<string> granfilhos = par[*it];

				for(set<string>::iterator it2 = granfilhos.begin(); it2!=granfilhos.end(); ++it2)
				{
					cout << *it2 << endl;
					for(int k =0; k<numSpacos2; k++)
						cout << " ";
				}
			}
			cout << endl;
			for(int k =0; k < numSpacos; k++) cout << " ";
		}
		cout << endl;
	}else
	{
		cout << "Erro: arvore muito grande." << endl;
	}
}
