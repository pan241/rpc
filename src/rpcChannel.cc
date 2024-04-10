#include "rpcChannel.h"
#include "RpcHeader.pb.h"
#include "rpcApplication.h"
#include "rpcController.h"

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

void rpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    uint32_t args_size;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error");
        exit(-1);
    }

    rpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string header_str;
    if (rpcHeader.SerializePartialToString(& header_str))
    {
        header_size = header_str.size();
    }
    else
    {
        controller->SetFailed("serialize header error");
        exit(-1);
    }

    std::string send_str;
    send_str.insert(0, std::string((char*)&header_size, 4));
    send_str += header_str;
    send_str += args_str;

    std::cout << "===================================" << std::endl;
    std::cout << "header size : " << header_size << std::endl;
    std::cout << "header str : " << header_str << std::endl;
    std::cout << "service name : " << service_name << std::endl;
    std::cout << "method name : " << method_name << std::endl;
    std::cout << "args str : " << args_str << std::endl;
    std::cout << "===================================" << std::endl;

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        exit(-1);
    }

    std::string ip = rpcApplication::getInstance().getConfig().load("rpcserverip");
    uint16_t port = atoi(rpcApplication::getInstance().getConfig().load("rpcserverport").c_str());

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(clientfd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "connect error : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    if (send(clientfd, send_str.c_str(), send_str.size(), 0) == -1)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "send error : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    char recv_buf[1024];
    int recv_size;
    if ((recv_size = recv(clientfd, recv_buf, 1024, 0)) == -1)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "receive error : %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    if (!response->ParseFromArray(recv_buf, recv_size))
    {
        controller->SetFailed("parse error");
        return;
    }

    close(clientfd);
}