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
        case 16:
            cmd16();
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
    string nicheng = c_r.nicheng();
    string email = c_r.email();
    string touxiang = c_r.touxiang();
    string message;
    bool isSuccess = _session->_server->database.registered(username, passwd, nicheng, email, touxiang, message);
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
    //设置自己->__withusername = 对方->_username;
    for (const auto &s:_session->_server->_cilentMap) {
        if (_session->_username == s.second->_withusername) {
            _session->_withusername = s.second->_username;
            s_g_isInvite.set_code(code);
            server_user_infor s_u_i;
            auto userGameInfor = _session->_server->database.getUserGameInfor(_session->_username);
            s_u_i.set_cmd(14);
            s_u_i.set_code(2);
            s_u_i.set_name(userGameInfor.name);
            s_u_i.set_lose(userGameInfor.lose);
            s_u_i.set_level(userGameInfor.level);
            s_u_i.set_draw(userGameInfor.draw);
            s_u_i.set_avatar(userGameInfor.avatar);
            s_u_i.set_win(userGameInfor.win);
            s_u_i.set_integral(userGameInfor.integral);
            s_u_i.set_gamecurrency(userGameInfor.gameCurrency);
            s_u_i.set_numsgame(userGameInfor.numsGame);
            s.second->writeData(s_u_i.SerializeAsString());
            auto userGameInfor2 = _session->_server->database.getUserGameInfor(_session->_withusername);
            s_u_i.set_cmd(14);
            s_u_i.set_code(2);
            s_u_i.set_name(userGameInfor2.name);
            s_u_i.set_lose(userGameInfor2.lose);
            s_u_i.set_level(userGameInfor2.level);
            s_u_i.set_draw(userGameInfor2.draw);
            s_u_i.set_avatar(userGameInfor2.avatar);
            s_u_i.set_win(userGameInfor2.win);
            s_u_i.set_integral(userGameInfor2.integral);
            s_u_i.set_gamecurrency(userGameInfor2.gameCurrency);
            s_u_i.set_numsgame(userGameInfor2.numsGame);
            _session->writeData(s_u_i.SerializeAsString());
            s.second->writeData(s_g_isInvite.SerializeAsString());
            break;
        }
    }
}

// 转发聊天消息
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

//保存对战结果
void Process::cmd15() {
    whoWin w_w;
    w_w.ParseFromArray(_buff.get(), _len);
    int code = w_w.code();
    auto withUserNameSession = _session->_server->findSession(_session->_withusername);

    if (code == 1) {
        _session->_server->database.winGame(_session->_username);
        _session->_server->database.loseGame(_session->_withusername);
    } else if (code == 2) {
        _session->_server->database.winGame(_session->_withusername);
        _session->_server->database.loseGame(_session->_username);
        w_w.set_win(_session->_withusername);
    } else if (code == 3) {
        //仅转发请求，服务器不做处理
    } else if (code == 4) {
        _session->_server->database.drawGame(_session->_withusername);
        _session->_server->database.drawGame(_session->_username);
    } else if (code == 5) {

    }
    _session->sendUserInfor();
    if (withUserNameSession != nullptr) {
        withUserNameSession->sendUserInfor();
        withUserNameSession->writeData(w_w.SerializeAsString());
    }
}

//悔棋
void Process::cmd16() {
    withDraw w_d;
    w_d.ParseFromArray(_buff.get(), _len);
    auto withUserNameSession = _session->_server->findSession(_session->_withusername);
    if (withUserNameSession != nullptr) {
        withUserNameSession->writeData(w_d.SerializeAsString());
    }
}
