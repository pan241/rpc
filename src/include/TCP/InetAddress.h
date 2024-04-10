#pragma once

#include <arpa/inet.h>
#include <string.h>
#include <string>

class InetAddress
{
public:
    explicit InetAddress(uint16_t port = 0, std::string ip = "127.0.0.1");

    explicit InetAddress(const sockaddr_in& addr) :
        _addr(addr)
    {}

    sa_family_t family() const { return _addr.sin_family; }

    std::string toIp()const;
    std::string toIpPort() const;
    uint16_t toPort() const;

    const sockaddr_in * getSockAddr() const { return &_addr; }
    void setSockAddr(const sockaddr_in& addr) { _addr = addr; }

private:
    sockaddr_in _addr;
};
