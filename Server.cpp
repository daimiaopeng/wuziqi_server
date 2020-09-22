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
            {
                lock_guard<mutex> lock(_mutex);
                _cilentMap[session] = "None";
            }
            session->start();
        }
        do_accept();
    });
}

void Server::manageClient() {
    LOG(INFO) << "veiwClient";
    thread t([this]() {
        while (1) {
            {
                list<string> onlinePeople;
                lock_guard<mutex> lock(_mutex);
                for (auto cilent = _cilentMap.begin(); cilent != _cilentMap.end(); ++cilent) {
                    if (cilent->first->isColse) {
                        LOG(INFO) << "用户" << cilent->first->_username << "断开连接";
                        _cilentMap.erase(cilent);
                    } else if (cilent->first->_username != "None") {
                        cilent->second = cilent->first->_username;
                        onlinePeople.push_back(cilent->second);
                    }
                }
                sendOnlineAll(onlinePeople);
//                LOG(INFO) << _cilentMap.size();
            }
            this_thread::sleep_for(chrono::seconds(1));
        }
    });
    t.detach();
}

void Server::sendOnlineAll(list<string> &onlinePeople) {
    server_online_infor s_o_i;
    s_o_i.set_cmd(9);
    for (auto &people:onlinePeople) {
        s_o_i.add_people(people);
    }
    for (auto cilent = _cilentMap.begin(); cilent != _cilentMap.end(); ++cilent) {
        if (cilent->first->_username != "None") {
            cilent->first->writeData(s_o_i.SerializeAsString());
        }
    }
}

void Server::run() {
    manageClient();
    do_accept();
}
