#include "test.pb.h"
#include <iostream>
#include <string>

using namespace fixbug;

void test1()
{
    LoginRequest req;
    req.set_name("name");
    req.set_pwd("123456");

    // serialize
    std::string send_str;
    if (req.SerializePartialToString(&send_str))
    {
        std::cout << send_str.c_str() << std::endl;
    }

    // deserialize
    LoginRequest req2;
    if (req2.ParseFromString(send_str))
    {
        std::cout << req2.name() << std::endl;
        std::cout << req2.pwd() << std::endl;
    }
}

void test2()
{
    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);
    rc->set_errmsg("noerror");

    User *user1 = rsp.add_friend_list();
    user1->set_name("pd");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    std::string usr_str;
    //std::cout << rsp.friend_list_size() << std::endl;
    if (rc->SerializeToString(&usr_str))
    {
        std::cout << usr_str << std::endl;
    }

    GetFriendListsResponse rsp2;
    if (rsp2.ParseFromString(usr_str))
    {
        std::cout << rsp2.result().SerializeAsString() << std::endl;
        //std::cout << req2.pwd() << std::endl;
    }
    
}

int main()
{
    test2();
    return 0;
}