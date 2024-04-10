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
        std::cout << "doing Login" << std::endl;
        return false;
    }

    bool Register(uint16_t id, std::string name, std::string pwd)
    {
        std::cout << "doing Register" << std::endl;
        std::cout << "id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
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
        code->set_errcode(1);
        code->set_message("test error");
        response->set_success(login_result);

        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                       const ::user::RegisterRequest* request,
                       ::user::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_message("");
        response->set_success(ret);

        done->Run();
    }

};

int main(int argc, char** argv)
{
    // initialization
    // provider -i config.conf
    rpcApplication::Init(argc, argv);

    // release service
    rpcProvider provider;
    provider.notifyService(new UserService());
    
    // start rpc node, after run, process is blocked until return
    provider.run();

    return 0;
}