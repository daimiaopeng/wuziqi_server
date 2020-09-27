//
// Created by daimiaopeng on 2020/9/20.
//

#include "Session.h"
#include "Process.h"


void Session::do_read_header() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(&header, MESSAGE_SIZE),
                            [this, self](boost::system::error_code ec, std::size_t len) {
                                if (!ec) {
                                    LOG(INFO) << " header.len:" << header.len;
                                    do_read_body(header.len);
                                } else {
                                    close();
                                    LOG(INFO) << "close() do_read_header ec: " << ec;
                                }
                            });
}

void Session::do_read_body(int dataLen) {
    auto self(shared_from_this());
    shared_ptr<char[]> buff(new char[dataLen], [](char *ptr) { delete[](ptr); });

    boost::asio::async_read(_socket, boost::asio::buffer(buff.get(), dataLen),
                            [this, self, buff](boost::system::error_code ec, std::size_t len) {
                                if (!ec) {
                                    Process process(self, buff, len);
                                    process.resolve();
                                } else {
                                    close();
                                    LOG(INFO) << "close() do_read_body ec: " << ec;
                                }
                                do_read_header();
                            });
}

void Session::writeData(string &&data) {
    auto self(shared_from_this());
    boost::asio::async_write(_socket, boost::asio::buffer(data.c_str(), data.length()),
                             [this, self](boost::system::error_code ec, std::size_t len) {
                                 if (!ec) {
                                     LOG(INFO) << "do_write 发送成功";
                                 } else {
                                     close();
                                     LOG(INFO) << "close() writeData ec: " << ec;
                                 }
                             });
}

void Session::close() {
    _socket.close();
//    LOG(INFO) << "用户"<<_username<<"断开连接";
    isColse = true;
}

Session::~Session() {
    close();
}