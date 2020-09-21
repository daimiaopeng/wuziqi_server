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
//        LOG(INFO) << "username:" << username << " passwd:" << passwd;
    string message;
    string token = _redis.login(username, passwd, message);
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
    bool isSuccess = _redis.registered(username, passwd, message);
    server_register s_r;
    s_r.set_cmd(5);
    s_r.set_issuccess(isSuccess);
    s_r.set_message(message);
    _session->writeData(s_r.SerializeAsString());
}

