cmake_minimum_required(VERSION 3.10)
project(wuziqi_server)

set(CMAKE_CXX_STANDARD 14)

set(Boost_INCLUDE_DIR /usr/local/include/boost)
set(Boost_LIBRARY_DIR /usr/local/lib)
include_directories('/usr/local/protobuf/include')
link_directories('/usr/local/protobuf/lib')


include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(wuziqi_server main.cpp Session.cpp Session.h protocol/Message.h Server.cpp Server.h protocol/base.pb.cc protocol/base.pb.h Process.cpp Process.h)


target_link_libraries(wuziqi_server pthread boost_system boost_thread boost_coroutine boost_date_time protoc protobuf protobuf-lite glog)
