#include <iostream>
#include <unistd.h>

#include "rpcApplication.h"

void showArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void rpcApplication::Init(int argc, char** argv)
{
    if (argc < 2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argc, "i,:")) != -1)
    {
        switch (c)
        {
        case 'i' :
            config_file = optarg;
            break;
        case '?' :
            std::cout << "invalid args!" << std::endl;
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':' :
            std::cout << "need <configfile>" << std::endl;
            showArgsHelp();
            exit(EXIT_FAILURE);
        default :
            break;
        }    
    }

}

rpcApplication& rpcApplication::getInstance()
{
    static rpcApplication app;
    return app;
}