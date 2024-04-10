#include <iostream>
#include "rpcApplication.h"
#include "rpcChannel.h"
#include "user.pb.h"

int main(int argc, char** argv)
{
    rpcApplication::Init(argc, argv);

    user::UserServiceRpc_Stub stub(new rpcChannel());
    user::LoginRequest request;
    request.set_name("pd");
    request.set_pwd("1234");

    user::LoginResponse response;
    rpcController controller;
    stub.Login(nullptr, &request, &response, nullptr);

    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login error: " << response.result().message() << std::endl;
    }

    user::RegisterRequest reg;
    reg.set_id(2000);
    reg.set_name("pd");
    reg.set_pwd("888888");
    user::RegisterResponse rsp;
    stub.Register(nullptr, &reg, &rsp, nullptr);

    if (0 == response.result().errcode())
    {
        std::cout << "rpc register response: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register error: " << response.result().message() << std::endl;
    }

    return 0;
}