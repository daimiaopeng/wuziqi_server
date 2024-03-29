//
// Created by daimiaopeng on 2020/9/20.
//


#ifndef WUZIQI_SERVER_SERVER_H
#define WUZIQI_SERVER_SERVER_H

#include <memory>
#include <utility>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <queue>
#include <map>
#include <mutex>
#include "Session.h"
#include "glog/logging.h"
#include "Database.h"

using namespace std;
using boost::asio::ip::tcp;

class Server : public enable_shared_from_this<Server> {
public:
    Server(boost::asio::io_context &ioContext, short port) : _ioContext(ioContext), _port(port),
                                                             _acceptor(ioContext,
                                                                       tcp::endpoint(tcp::v4(), port)) {

    }

    void run();

    shared_ptr<Session> findSession(const string &name);

    void startGame(shared_ptr<Session> play1, shared_ptr<Session> play2);


private:
    void do_accept();

    void manageClient();

    void sendOnlineAll();


public:
    mutex _mutex;
    map<string, shared_ptr<Session>> _cilentMap;
    boost::asio::io_context &_ioContext;
    tcp::acceptor _acceptor;
    short _port;
    Database _database;
};


#endif //WUZIQI_SERVER_SERVER_H