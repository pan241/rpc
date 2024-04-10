#ifndef RPC_CONFIG_H
#define RPC_CONFIG_H

#include <unordered_map>
#include <string>

// ip port zookeeperip zookeeperport

class rpcConfig
{
public:
    void loadConfigFile(const char* config_file);

    std::string load(const std::string& key);

private:
    std::unordered_map<std::string, std::string> _configMap;
};

#endif