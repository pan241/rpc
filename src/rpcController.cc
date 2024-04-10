#include "rpcController.h"

rpcController::rpcController()
: failed_(false), errMsg_("")
{

}

void rpcController::Reset()
{
    failed_ = false;
    errMsg_ = "";
}

bool rpcController::Failed() const
{
    return failed_;
}

std::string rpcController::ErrorText() const
{
    return errMsg_;
}

void rpcController::SetFailed(const std::string& reason)
{
    failed_ = true;
    errMsg_ = reason;
}

void rpcController::StartCancel() {}
bool rpcController::IsCanceled() const { return false; }
void rpcController::NotifyOnCancel(google::protobuf::Closure* callback) {}