#pragma once

#include "google/protobuf/service.h"
#include <memory>
#include <string>
#include <unordered_map>

#include "EventLoop.h"
#include "TcpServer.h"
#include "InetAddress.h"

class rpcProvider
{
public:
    void notifyService(google::protobuf::Service* service);

    void run();

private:
    EventLoop eventloop_;

    struct ServiceInfo
    {
        google::protobuf::Service* service_;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap_;
    };

    std::unordered_map<std::string, ServiceInfo> serviceMap_;

    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    void RpcResponse(const TcpConnectionPtr&, google::protobuf::Message*);
};
