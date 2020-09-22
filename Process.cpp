//
// Created by daimiaopeng on 2020/9/20.
//

#include "Process.h"

void Process::cmd0() {
    test t;
    t.ParseFromArray(_buff.get(), _len);
    LOG(INFO) << "cmd 0 string:" << t.inform();
}

void Process::cmd2() {
    client_login c_l;
    c_l.ParseFromArray(_buff.get(), _len);
    string username = c_l.username();
    string passwd = c_l.passwd();
    string message;
    string token = _session->_server->_redis.login(username, passwd, message);
    if (token != "") {
        _session->_username = username;
    }
    server_login s_l;
    s_l.set_cmd(3);
    s_l.set_token(token);
    s_l.set_message(message);
    _session->writeData(s_l.SerializeAsString());
}

void Process::cmd4() {
    client_register c_r;
    c_r.ParseFromArray(_buff.get(), _len);
    string username = c_r.username();
    string passwd = c_r.passwd();
    string message;
    bool isSuccess = _session->_server->_redis.registered(username, passwd, message);
    server_register s_r;
    s_r.set_cmd(5);
    s_r.set_issuccess(isSuccess);
    s_r.set_message(message);
    _session->writeData(s_r.SerializeAsString());
}

void Process::cmd6() {
    client_gobang_position c_g_p;
    c_g_p.ParseFromArray(_buff.get(), _len);
    int x = c_g_p.x();
    int y = c_g_p.y();

    server_gobang_position s_g_p;
    s_g_p.set_cmd(7);
    s_g_p.set_x(x);
    s_g_p.set_y(y);
    for (auto &s:_session->_server->_cilentMap) {
        if (_session->withusername == s.first->_username) {
            s.first->writeData(s_g_p.SerializeAsString());
            break;
        }
    }
}

void Process::cmd8() {
    client_create_game c_c_g;
    c_c_g.ParseFromArray(_buff.get(), _len);
    _session->withusername = c_c_g.withusername();
    lock_guard<mutex> lock(_session->_server->_mutex);
    for (auto &s:_session->_server->_cilentMap) {
        if (s.first->_username == _session->withusername) {
            server_game_invite s_g_i;
            s_g_i.set_cmd(10);
            LOG(INFO) << "set_cmd ";
            s_g_i.set_people(_session->_username);
            s.first->writeData(s_g_i.SerializeAsString());
            break;
        }
    }
}

void Process::cmd11() {
    client_game_invite c_g_i;
    c_g_i.ParseFromArray(_buff.get(), _len);
    int code = c_g_i.code();
    server_game_isInvite s_g_isInvite;
    s_g_isInvite.set_cmd(12);
    lock_guard<mutex> lock(_session->_server->_mutex);
    for (auto &s:_session->_server->_cilentMap) {
        if (_session->_username == s.first->withusername) {
            _session->withusername = s.first->_username;
            s_g_isInvite.set_code(code);
            s.first->writeData(s_g_isInvite.SerializeAsString());
            break;
        }
    }
}