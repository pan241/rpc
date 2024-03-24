#ifndef RPCPROVIDER_H
#define RPCPROVIDER_H

#include "google/protobuf/service.h"

class rpcProvider
{
public:
    void notifyService(google::protobuf::Service* service);

    void run();
};

#endif