#include <iostream>
#include <unistd.h>

#include "rpcApplication.h"

rpcConfig rpcApplication::config_;

void showArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

rpcApplication::rpcApplication()
{

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
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i' :
            config_file = optarg;
            break;
        case '?' :
        case ':' :
            showArgsHelp();
            exit(EXIT_FAILURE);
        default :
            break;
        }    
    }

    config_.loadConfigFile(config_file.c_str());
}

rpcApplication& rpcApplication::getInstance()
{
    static rpcApplication app;
    return app;
}

rpcConfig& rpcApplication::getConfig()
{
    return config_;
}