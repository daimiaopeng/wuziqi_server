//
// Created by daimiaopeng on 2020/10/14.
//

#include "Database.h"

//初始化用户游戏信息
void Database::initUserGameInfor(const string &name) {
    User user = getUserInfor(name);
    UserGameInfor userGameInfor{-1, name, 0, 0, 0, 0, 0, 0, 0, user.touxiang};
    userGameInfor.id = _storage.insert(userGameInfor);
}

//注册初始化用户信息
bool Database::registered(const string &name, const string &passwd, const string &nicheng, const string &email,
                          const string &touxiang, string &message) {
    auto res = _storage.get_all<User>(where(c(&User::name) == name));
    if (!res.empty()) {
        message = "该用户已注册";
        return false;
    }
    User user{-1, name, passwd, nicheng, email, touxiang};
    user.id = _storage.insert(user);
    message = "注册成功";
    return true;
}

//登录判断
bool Database::login(const string &name, const string &passwd, string &message) {
    auto res = _storage.get_all<User>(where(c(&User::name) == name));
    if (res.empty()) {
        message = "该用户没有注册";
        return false;
    }
    if (res[0].passwd == passwd) {
        message = "登录成功";
        return true;
    } else {
        message = "登录密码错误";
        return false;
    }
}

//获取用户游戏信息
UserGameInfor Database::getUserGameInfor(const string &name) {
    auto res = _storage.get_all<UserGameInfor>(where(c(&UserGameInfor::name) == name));
    if (res.empty()) {
        return {};
    }
    return res[0];
}

//获取用户信息
User Database::getUserInfor(const string &name) {
    auto res = _storage.get_all<User>(where(c(&User::name) == name));
    if (res.empty()) {
        return {};
    }
    return res[0];
}

//用户赢了游戏后数据库操作
void Database::winGame(const string &name) {
    auto res = _storage.get_all<UserGameInfor>(where(c(&UserGameInfor::name) == name));
    if (res.empty()) {
        return;
    }
    UserGameInfor &userGameInfor = res[0];
    userGameInfor.numsGame++;
    userGameInfor.win++;
    userGameInfor.integral = userGameInfor.integral + 5;
    _storage.update(userGameInfor);
}

//用户输了游戏后数据库操作
void Database::loseGame(const string &name) {
    auto res = _storage.get_all<UserGameInfor>(where(c(&UserGameInfor::name) == name));
    if (res.empty()) {
        return;
    }
    UserGameInfor &userGameInfor = res[0];
    userGameInfor.numsGame++;
    userGameInfor.lose++;
    userGameInfor.integral = userGameInfor.integral - 3;
    _storage.update(userGameInfor);
}

//用户和棋游戏后数据库操作
void Database::drawGame(const string &name) {
    auto res = _storage.get_all<UserGameInfor>(where(c(&UserGameInfor::name) == name));
    if (res.empty()) {
        return;
    }
    UserGameInfor &userGameInfor = res[0];
    userGameInfor.numsGame++;
    userGameInfor.draw++;
    userGameInfor.integral = userGameInfor.integral + 2;
    _storage.update(userGameInfor);
}

//用户登录记录
void Database::signInLog(const string &name) {
    std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
    int pos = strTime.find('T');
    strTime.replace(pos, 1, std::string("-"));
    strTime.replace(pos + 3, 0, std::string(":"));
    strTime.replace(pos + 6, 0, std::string(":"));
    SignInLog signIn{-1, name, strTime};
    _storage.insert(signIn);
}