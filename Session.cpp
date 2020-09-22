//
// Created by daimiaopeng on 2020/9/20.
//

#include "Session.h"
#include "Process.h"

void Session::do_read_body(int dataLen) {
    auto self(shared_from_this());
    shared_ptr<char[]> buff(new char[dataLen], [](char *ptr) { delete[](ptr); });

    boost::asio::async_read(_socket, boost::asio::buffer(buff.get(), dataLen),
                            [this, self, buff](boost::system::error_code ec, std::size_t len) {
                                if (!ec) {
                                    Process process(self, buff, len);
                                } else {
                                    close();
                                    LOG(INFO) << "close() do_read_body ec: " << ec;
                                }
                                do_read_header();
                            });
}

Session::~Session() {
    close();
}

void Session::close() {
    _socket.close();
//    LOG(INFO) << "用户"<<_username<<"断开连接";
    isColse = true;
}
