//
// Created by daimiaopeng on 2020/9/20.
//


#ifndef WUZIQI_SERVER_SESSION_H
#define WUZIQI_SERVER_SESSION_H

#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include "protocol/Message.h"
#include "glog/logging.h"
#include "protocol/base.pb.h"
#include "Redis.h"

using namespace std;
using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socket_ptr;


class Process;

class Server;

class Session : public enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, shared_ptr<Server> server) : _socket(std::move(socket)), _server(server) {}

    void start() {
        isColse = false;
        _username = "None";
        do_read_header();
    }

    ~Session();

private:
    void do_read_header();

    void do_read_body(int dataLen);

public:
    void writeData(string &&data);

    void close();

public:
    string _withusername;
    bool isColse;
    string gameId;
    string token;
    shared_ptr<Server> _server;
    string _username;
    tcp::socket _socket;
    struct message header;
    int MESSAGE_SIZE = sizeof(message);
};


#endif //WUZIQI_SERVER_SESSION_H