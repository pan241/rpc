#pragma once

#include "rpcConfig.h"
#include "rpcProvider.h"
#include "rpcController.h"

// 大量共享信息，设置为单例模式
class rpcApplication
{
public:
    static void Init(int argc, char** argv);
    static rpcApplication& getInstance();
    static rpcConfig& getConfig();

private:
    static rpcConfig config_;

    rpcApplication();
    rpcApplication(const rpcApplication&) = delete;
    rpcApplication(rpcApplication&&) = delete;
};
