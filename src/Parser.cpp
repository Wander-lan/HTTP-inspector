#include "Parser.hpp"

std::set<std::string> Parser::verifica_url(const char *corpo)
{
    using       namespace std;
    string      str(corpo);
    set<string> urls;
    try
    {
        regex r("href=\"(.*?)\"", regex_constants::icase);
        urls = {sregex_token_iterator(str.begin(), str.end(), r, 1), sregex_token_iterator{}};
    }catch (...){;
    }
    return urls;
}

std::string Parser::verifica_html(const char *data)
{
    using  namespace std;
    string str(data);
    int    from = str.find("<!DOCTYPE");

    if(from > str.length())
       int from =str.find("<html");

    int to = str.find("</html>");

    if(from>=str.length())
        return str;

    if(to>=str.length())
        return str.substr(from, str.length()-from);

    return str.substr(from, to-from+7);
}


std::set<std::string> Parser::verifica_emissor(const char *corpo)
{
    using namespace std;
    string          str(corpo);
    set<string>     sources;
    try
    {
        regex r("src=\"(.*?)\"", regex_constants::icase);
        sources = {sregex_token_iterator(str.begin(), str.end(), r, 1), sregex_token_iterator{}};
    }catch(...){;
    }
    return sources;
}


std::set<std::string> Parser::verifica_import(const char *corpo)
{
    using namespace std;
    string          str(corpo);
    set<string>     imports;
    try
    {
        regex r("url(\"(.*?)\"", regex_constants::icase);
        imports = {sregex_token_iterator(str.begin(), str.end(), r, 1), sregex_token_iterator{}};
    }catch(...){;
    }
    return imports;
}
