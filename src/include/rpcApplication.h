#ifndef RPCAPPLICATION_H
#define RPCAPPLICATION_H

// 大量共享信息，设置为单例模式
class rpcApplication
{
public:
    static void Init(int argc, char** argv);
    static rpcApplication& getInstance();

private:
    rpcApplication(){}
    rpcApplication(const rpcApplication&) = delete;
    rpcApplication(rpcApplication&&) = delete;
};

#endif