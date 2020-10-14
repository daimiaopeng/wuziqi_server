//
// Created by daimiaopeng on 2020/9/20.
//

#include "Server.h"

void Server::do_accept() {
    auto self(shared_from_this());
    _acceptor.async_accept([this, self](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            LOG(INFO) << "有客户端连接";
            auto session(std::make_shared<Session>(std::move(socket), self));
            session->start();
        }
        do_accept();
    });
}

void Server::manageClient() {
    LOG(INFO) << "manageClient线程运行中";
    thread t([this]() {
        while (1) {
            {
                sendOnlineAll();
            }
            this_thread::sleep_for(chrono::seconds(1));
        }
    });
    t.detach();
}

void Server::sendOnlineAll() {
    server_online_infor s_o_i;
    s_o_i.set_cmd(9);
    for (auto &people:_cilentMap) {
        s_o_i.add_people(people.first);
    }
    lock_guard<mutex> lock(_mutex);
    //    LOG(INFO) << _cilentMap.size();

    for (const auto &cilent:_cilentMap) {
        cilent.second->writeData(s_o_i.SerializeAsString());
    }
}

void Server::run() {
    manageClient();
    do_accept();
}
