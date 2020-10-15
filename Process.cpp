//
// Created by daimiaopeng on 2020/9/20.
//

#include "Process.h"

void Process::resolve() {
    cmd c;
    c.ParseFromArray(_buff.get(), _len);
    LOG(INFO) << "cmd:" << c.c();
    switch (c.c()) {
        case 0:
            cmd0();
            break;
        case 2:
            cmd2();
            break;
        case 4:
            cmd4();
            break;
        case 6:
            cmd6();
            break;
        case 8:
            cmd8();
            break;
        case 11:
            cmd11();
            break;
        case 13:
            cmd13();
            break;
        case 15:
            cmd15();
            break;
        default:
            LOG(INFO) << "cmd default";
    }
}

// 测试
void Process::cmd0() {
    test t;
    t.ParseFromArray(_buff.get(), _len);
    LOG(INFO) << "cmd 0 string:" << t.inform();
}

//登录
void Process::cmd2() {
    client_login c_l;
    c_l.ParseFromArray(_buff.get(), _len);
    string username = c_l.username();
    string passwd = c_l.passwd();
    string message;
    bool code = _session->_server->database.login(username, passwd, message);
    if (code) {
        _session->_username = username;
        lock_guard<mutex> lock(_session->_server->_mutex);
        _session->_server->_cilentMap[username] = _session;
    }
    server_login s_l;
    s_l.set_cmd(3);
    s_l.set_issuccess(code);
    s_l.set_message(message);
    _session->writeData(s_l.SerializeAsString());
    if (code) {
        _session->sendUserInfor();
    }
}

//注册
void Process::cmd4() {
    client_register c_r;
    c_r.ParseFromArray(_buff.get(), _len);
    string username = c_r.username();
    string passwd = c_r.passwd();
    string message;
    bool isSuccess = _session->_server->database.registered(username, passwd, message);
    server_register s_r;
    s_r.set_cmd(5);
    s_r.set_issuccess(isSuccess);
    s_r.set_message(message);
    _session->writeData(s_r.SerializeAsString());
    if (isSuccess) {
        _session->_server->database.initUserGameInfor(username);
    }
}

// 发送棋的位置
void Process::cmd6() {
    client_gobang_position c_g_p;
    c_g_p.ParseFromArray(_buff.get(), _len);
    int x = c_g_p.x();
    int y = c_g_p.y();
    server_gobang_position s_g_p;
    s_g_p.set_cmd(7);
    s_g_p.set_x(x);
    s_g_p.set_y(y);
    sendOne(_session->_withusername, s_g_p.SerializeAsString());
}

// 发送对战请求
void Process::cmd8() {
    client_create_game c_c_g;
    c_c_g.ParseFromArray(_buff.get(), _len);
    _session->_withusername = c_c_g.withusername();
    server_game_invite s_g_i;
    s_g_i.set_cmd(10);
    LOG(INFO) << _session->_withusername;
    s_g_i.set_people(_session->_username);
    sendOne(_session->_withusername, s_g_i.SerializeAsString());
}

//是否接受玩家游戏邀请(回复)
void Process::cmd11() {
    client_game_invite c_g_i;
    c_g_i.ParseFromArray(_buff.get(), _len);
    int code = c_g_i.code();
    server_game_isInvite s_g_isInvite;
    s_g_isInvite.set_cmd(12);
    lock_guard<mutex> lock(_session->_server->_mutex);
    for (auto &s:_session->_server->_cilentMap) {
        if (_session->_username == s.second->_withusername) {
            _session->_withusername = s.second->_username;
            s_g_isInvite.set_code(code);
            s.second->writeData(s_g_isInvite.SerializeAsString());
            break;
        }
    }
}

// 聊天消息
void Process::cmd13() {
    sendOne(_session->_withusername, string(_buff.get(), _len));
}

void Process::sendOne(const string &name, const string &data) {
    lock_guard<mutex> lock(_session->_server->_mutex);
    auto res = _session->_server->_cilentMap.find(name);
    if (res == _session->_server->_cilentMap.end()) {
        return;
    }
    res->second->writeData(data);
}

void Process::cmd15() {
    whoWin w_w;
    w_w.ParseFromArray(_buff.get(), _len);
    int code = w_w.code();
    if (code == 1) {
        _session->_server->database.winGame(_session->_username);
        _session->_server->database.loseGame(_session->_withusername);
        _session->sendUserInfor();
        auto withUserNameSession = _session->_server->findSession(_session->_withusername);
        if (withUserNameSession != nullptr) {
            withUserNameSession->sendUserInfor();
        }
    } else if (code == 2) {

    } else {

    }
}
