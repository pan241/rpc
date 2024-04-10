#pragma once

#include <google/protobuf/service.h>
#include <string>

class rpcController : public google::protobuf::RpcController
{
public:
    rpcController();
    void Reset() override;
    bool Failed() const override;
    std::string ErrorText() const override;
    void SetFailed(const std::string& reason) override;

    void StartCancel() override;
    bool IsCanceled() const override;
    void NotifyOnCancel(google::protobuf::Closure* callback) override;
private:
    bool failed_;
    std::string errMsg_;

};