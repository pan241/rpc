#include <string>
#include <iostream>

#include "user.pb.h"
#include "rpcApplication.h"
#include "rpcProvider.h"

//local service
class UserService : public user::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "do Login" << std::endl;
        return true;
    }

    // service provider
    // caller send request -> net -> callee receive -> override service
    void Login(::google::protobuf::RpcController* controller,
                       const ::user::LoginRequest* request,
                       ::user::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);

        user::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_message("");
        response->set_success(login_result);

        done->Run();
    }

};

int main(int argc, char** argv)
{
    // initialization
    rpcApplication::Init(argc, argv);

    // release service
    rpcProvider provider;
    provider.notifyService(new UserService());
    
    // start rpc node, after run, process is blocked until return
    provider.run();

    return 0;
}