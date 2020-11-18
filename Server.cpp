//
// Created by daimiaopeng on 2020/9/20.
//

#include "Server.h"

//接收连接，保存session
void Server::do_accept() {
    auto self(shared_from_this());
    _acceptor.async_accept([this, self](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            LOG(INFO) << "Client connection";
            auto session(std::make_shared<Session>(std::move(socket), self));
            session->start();
        }
        do_accept();
    });
}

//单独的session管理线程
void Server::manageClient() {
    LOG(INFO) << "Client management thread is running";
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

//发送在线的人信息，即在线列表
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

//运行服务
void Server::run() {
    manageClient();
    do_accept();
}

//找到指定用户session
shared_ptr<Session> Server::findSession(const string &name) {
    lock_guard<mutex> lock(_mutex);
    auto res = _cilentMap.find(name);
    if (res == _cilentMap.end()) {
        return nullptr;
    }
    return res->second;
}

//开始游戏时应该发送的数据包
void Server::startGame(shared_ptr<Session> play1, shared_ptr<Session> play2) {
    auto sendUserGameInfor = [&](shared_ptr<Session> play1, shared_ptr<Session> play2) {
        server_user_infor s_u_i;
        auto userGameInfor = _database.getUserGameInfor(play1->_username);
        s_u_i.set_cmd(14);
        s_u_i.set_code(1);
        s_u_i.set_name(userGameInfor.name);
        s_u_i.set_lose(userGameInfor.lose);
        s_u_i.set_level(userGameInfor.level);
        s_u_i.set_draw(userGameInfor.draw);
        s_u_i.set_avatar(userGameInfor.avatar);
        s_u_i.set_win(userGameInfor.win);
        s_u_i.set_integral(userGameInfor.integral);
        s_u_i.set_gamecurrency(userGameInfor.gameCurrency);
        s_u_i.set_numsgame(userGameInfor.numsGame);
        play1->writeData(s_u_i.SerializeAsString());
        s_u_i.set_code(2);
        play2->writeData(s_u_i.SerializeAsString());
    };
    sendUserGameInfor(play1, play2);
    sendUserGameInfor(play2, play1);
}