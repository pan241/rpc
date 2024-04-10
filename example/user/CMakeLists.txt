set(SRC_LIST User.cc ../user.pb.cc)
add_executable(user ${SRC_LIST})

target_link_libraries(user rpc protobuf tcp_server)