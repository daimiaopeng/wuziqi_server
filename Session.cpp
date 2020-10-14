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
//                                    LOG(INFO) << " header.len:" << header.len;
                                    do_read_body(header.len);
                                } else {
                                    close();
//                                    LOG(INFO) << "close() do_read_header ec: " << ec;
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

void Session::writeData(string data) {
    auto self(shared_from_this());
    boost::asio::async_write(_socket, boost::asio::buffer(data.c_str(), data.length()),
                             [this, self](boost::system::error_code ec, std::size_t len) {
                                 if (!ec) {
//                                     LOG(INFO) << "do_write 发送成功";
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
    lock_guard<mutex> lock(_server->_mutex);
    _server->_cilentMap.erase(_username);
}

Session::~Session() {
    close();

    LOG(INFO) << "客户端关闭";
}

void Session::sendUserInfor() {
    server_user_infor s_u_i;
    auto res = _server->database._storage.get_all<UserGameInfor>(where(c(&UserGameInfor::name) == _username));
    if (res.empty()) return;
    s_u_i.set_cmd(14);
    s_u_i.set_name(res[0].name);
    s_u_i.set_lose(res[0].lose);
    s_u_i.set_level(res[0].level);
    s_u_i.set_draw(res[0].draw);
    s_u_i.set_avatar(res[0].avatar);
    s_u_i.set_win(res[0].win);
    s_u_i.set_integral(res[0].integral);
    s_u_i.set_gamecurrency(res[0].gameCurrency);
    s_u_i.set_numsgame(res[0].numsGame);
    writeData(s_u_i.SerializeAsString());
}