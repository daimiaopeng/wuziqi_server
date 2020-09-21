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

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Redis &redis) : _socket(std::move(socket)), _redis(redis) {}

    void start() {
        do_read_header();
    }

private:
    void do_read_header() {
        auto self(shared_from_this());
        _socket.async_read_some(boost::asio::buffer(&header, MESSAGE_SIZE),
                                [this, self](boost::system::error_code ec, std::size_t len) {
                                    if (!ec) {
                                        LOG(INFO) << " header.len:" << header.len;
                                        do_read_body(header.len);
//                                      do_write(len);
                                    } else {
                                        _socket.close();
                                        LOG(INFO) << "_socket.close() do_read_header ec: " << ec;
                                    }
                                });
    }

    void do_read_body(int dataLen);

public:
    void writeData(string &&data) {
        auto self(shared_from_this());
        boost::asio::async_write(_socket, boost::asio::buffer(data.c_str(), data.length()),
                                 [this, self](boost::system::error_code ec, std::size_t len) {
                                     if (!ec) {
                                         LOG(INFO) << "do_write 发送成功";
                                     }
                                 });
    }

    Redis &_redis;
    tcp::socket _socket;
    struct message header;
    static const int MESSAGE_SIZE = sizeof(message);
};


#endif //WUZIQI_SERVER_SESSION_H