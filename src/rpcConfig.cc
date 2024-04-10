#include "rpcConfig.h"

#include <iostream>

void rpcConfig::loadConfigFile(const char* config_file)
{
    FILE* pfd = fopen(config_file, "r");
    if (pfd == nullptr)
    {
        std::cout << config_file << " is not exit" << std::endl;
        exit(EXIT_FAILURE);
    }

    // notation, correct config, remove space
    while (!feof(pfd))
    {
        char buf[512] = {0};   
        fgets(buf, 512, pfd);

        std::string src_buf(buf);
        int idx = src_buf.find_first_not_of(' ');
        if (idx != -1)
        {
            src_buf = src_buf.substr(idx, src_buf.size() - idx);
        }
        
        idx = src_buf.find_last_not_of(' ');
        if (idx != -1)
        {
            src_buf = src_buf.substr(0, idx + 1);
        }
        if (src_buf[0] == '#' || src_buf.empty())
        {
            continue;
        }
        idx = src_buf.find('=');
        if (idx == -1)
        {
            continue;
        }
        
        std::string key(src_buf.substr(0, idx));
        std::string value(src_buf.substr(idx + 1, src_buf.size() - idx));
        _configMap[key] = value;
    }
}

std::string rpcConfig::load(const std::string& key)
{
    if (_configMap.count(key))
    {
        return _configMap[key];
    }
    else
    {
        return "";
    }
}