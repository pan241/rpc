#include "rpcProvider.h"
#include "rpcApplication.h"
#include <functional>
#include <google/protobuf/descriptor.h>
#include "RpcHeader.pb.h"

/*


*/
void rpcProvider::notifyService(google::protobuf::Service* service)
{
    ServiceInfo serviceInfo;

    const google::protobuf::ServiceDescriptor* servicePtr = service->GetDescriptor();

    std::string service_name = servicePtr->name();
    std::cout << service_name << " ";

    int methodCnt = servicePtr->method_count();
    for (int i = 0; i < methodCnt; i++)
    {
        const google::protobuf::MethodDescriptor* methodPtr = servicePtr->method(i);
        std::string methodName = methodPtr->name();
        std::cout << methodName << " ";
        serviceInfo.methodMap_.insert({methodName, methodPtr});
    }
    serviceInfo.service_ = service;
    serviceMap_.insert({service_name, serviceInfo});

    std::cout << std::endl;
}

void rpcProvider::run()
{
    std::string ip = rpcApplication::getInstance().getConfig().load("rpcserverip");
    uint16_t port = atoi(rpcApplication::getInstance().getConfig().load("rpcserverport").c_str());
    InetAddress address(port, ip);

    TcpServer server(&eventloop_, address, "rpcProvider");
    server.setConnectionCallback(std::bind(&rpcProvider::onConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&rpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    server.setThreadNum(4);

    server.start();
    eventloop_.loop();
}

// 新的socket连接回调
void rpcProvider::onConnection(const TcpConnectionPtr& conn)
{
    if (!conn->connected())
    {
        conn->shutdown();
    }
}

/*
provider - comsumer
service_name + method_name + args_size
header_size + header_str + 
*/
// 读写事件回调
void rpcProvider::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp timestamp)
{
    std::string recv_buf = buf->retrieveAllAsString();

    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    std::string header_str = recv_buf.substr(4, header_size);
    rpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(header_str))
    {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        std::cout << "parse error!" << std::endl;
        exit(-1);
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    auto iter = serviceMap_.find(service_name); 
    if (iter == serviceMap_.end())
    {
        exit(-1);
    }
    if (!iter->second.methodMap_.count(method_name))
    {
        exit(-1);
    }

    google::protobuf::Service* service = iter->second.service_;
    const google::protobuf::MethodDescriptor* method = iter->second.methodMap_[method_name];

    // 获得request和response参数
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str))
    {
        exit(-1);
    }
    google::protobuf::Message* response = service->GetResponsePrototype(method).New();

    google::protobuf::Closure* done = google::protobuf::NewCallback<rpcProvider, const TcpConnectionPtr&, google::protobuf::Message*>(this, &rpcProvider::RpcResponse, conn, response);

    service->CallMethod(method, nullptr, request, response, done);

    std::cout << "===================================" << std::endl;
    std::cout << "header size : " << header_size << std::endl;
    std::cout << "header str : " << header_str << std::endl;
    std::cout << "service name : " << service_name << std::endl;
    std::cout << "method name : " << method_name << std::endl;
    std::cout << "args str : " << args_str << std::endl;
    std::cout << "===================================" << std::endl;

}

void rpcProvider::RpcResponse(const TcpConnectionPtr& conn, google::protobuf::Message* response)
{
    std::string response_str;
    if (response->SerializePartialToString(&response_str))
    {
        conn->send(response_str);
    }
    else
    {
        exit(-1);
    }
    conn->shutdown();
}