#ifndef WUZIQI_SERVER_SERVER_H
#define WUZIQI_SERVER_SERVER_H

#include <memory>
#include <utility>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "Session.h"
#include "glog/logging.h"
#include "Redis.h"


using namespace std;
using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
//vector<shared_ptr<Session>> cilentList;


typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Server {
public:
    Server(boost::asio::io_context &ioContext, short port, Redis &redis) : _ioContext(ioContext), _port(port),
                                                                           _acceptor(ioContext,
                                                                                     tcp::endpoint(tcp::v4(), port)),
                                                                           _redis(redis) {
        do_accept();
    }


private:
    void do_accept();

    Redis &_redis;
    boost::asio::io_context &_ioContext;
    tcp::acceptor _acceptor;
    short _port;
};


#endif //WUZIQI_SERVER_SERVER_H