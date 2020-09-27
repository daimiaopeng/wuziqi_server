cmake_minimum_required(VERSION 3.10)
project(wuziqi_server)

set(CMAKE_CXX_STANDARD 14)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(Boost_INCLUDE_DIR /usr/local/include/boost)
    set(Boost_LIBRARY_DIR /usr/local/lib)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(Boost_INCLUDE_DIR "D:\\PATH\\boost_1_70_0")
    set(Boost_LIBRARY_DIR "D:\\PATH\\boost_1_70_0\\lib64-msvc-14.2")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

#set(CMAKE_CXX_FLAGS "--coverage")

include_directories('/usr/local/protobuf/include')
link_directories('/usr/local/protobuf/lib')


include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(wuziqi_server main.cpp Session.cpp Session.h protocol/Message.h Server.cpp Server.h protocol/base.pb.cc protocol/base.pb.h Process.cpp Process.h Redis.cpp Redis.h tool/md5.cpp tool/md5.h Game.cpp Game.h)


target_link_libraries(wuziqi_server pthread boost_system boost_thread boost_coroutine boost_date_time protoc protobuf protobuf-lite glog hiredis)
