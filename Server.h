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
#include <list>
#include <map>
#include <mutex>
#include "Session.h"
#include "glog/logging.h"
#include "Redis.h"


using namespace std;
using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Server : public enable_shared_from_this<Server> {
public:
    Server(boost::asio::io_context &ioContext, short port, Redis &redis) : _ioContext(ioContext), _port(port),
                                                                           _acceptor(ioContext,
                                                                                     tcp::endpoint(tcp::v4(), port)),
                                                                           _redis(redis) {

    }

    void run();


private:
    void do_accept();

    void manageClient();

    void sendOnlineAll(list<string> &onlinePeople);

public:
    mutex _mutex;
    map<shared_ptr<Session>, string> _cilentMap;
    Redis &_redis;
    boost::asio::io_context &_ioContext;
    tcp::acceptor _acceptor;
    short _port;
};


#endif //WUZIQI_SERVER_SERVER_H