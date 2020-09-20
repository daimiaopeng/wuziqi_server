//
// Created by daimiaopeng on 2020/9/20.
//

#include "Server.h"

void Server::do_accept() {
    _acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            LOG(INFO) << "有客户端连接";
            auto temp(std::make_shared<Session>(std::move(socket)));
//                cilentList.push_back(temp);
            temp->start();
        }
        do_accept();
    });
}