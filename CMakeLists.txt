cmake_minimum_required(VERSION 3.10)
project(wuziqi_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(Boost_INCLUDE_DIR /usr/local/include/boost)
    set(Boost_LIBRARY_DIR /usr/local/lib)
    include_directories('/usr/local/protobuf/include')
    link_directories('/usr/local/protobuf/lib')
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(Boost_INCLUDE_DIR "D:\\PATH\\boost_1_70_0")
    set(Boost_LIBRARY_DIR "D:\\PATH\\boost_1_70_0\\lib64-msvc-14.2")
    find_package(sqlite3 CONFIG REQUIRED)
    find_package(protobuf CONFIG REQUIRED)
    find_package(glog CONFIG REQUIRED)
    include_directories(D:/PATH/vcpkg/packages/sqlite-orm_x64-windows/include/sqlite_orm)
    include_directories(D:/PATH/vcpkg/packages/hiredis_x64-windows/include)
    link_directories(D:/PATH/vcpkg/packages/hiredis_x64-windows/lib)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

#set(CMAKE_CXX_FLAGS "--coverage")


include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_executable(wuziqi_server main.cpp Session.cpp Session.h protocol/Message.h Server.cpp Server.h protocol/linux/base.pb.cc protocol/linux/base.pb.h Process.cpp Process.h Redis.cpp Redis.h tool/md5.cpp tool/md5.h Game.cpp Game.h Database.cpp Database.h)
    target_link_libraries(wuziqi_server pthread boost_system boost_thread boost_coroutine boost_date_time protoc protobuf protobuf-lite glog hiredis sqlite3)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_executable(wuziqi_server main.cpp Session.cpp Session.h protocol/Message.h Server.cpp Server.h protocol/windows/base.pb.cc protocol/windows/base.pb.h Process.cpp Process.h Redis.cpp Redis.h tool/md5.cpp tool/md5.h Game.cpp Game.h Database.cpp Database.h)
    target_link_libraries(wuziqi_server PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite glog::glog hiredis sqlite3)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")