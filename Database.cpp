//
// Created by daimiaopeng on 2020/10/14.
//

#include "Database.h"

void Database::initUserGameInfor(const string &name) {
    UserGameInfor userGameInfor{-1, name, 0, 0, 0, 0, 0, 0, 0, ""};
    userGameInfor.id = _storage.insert(userGameInfor);

}

//注册
bool Database::registered(const string &name, const string &passwd, string &message) {
    auto res = _storage.get_all<User>(where(c(&User::name) == name));
    if (!res.empty()) {
        message = "该用户已注册";
        return false;
    }
    User user{-1, name, passwd};
    user.id = _storage.insert(user);
    message = "注册成功";
    return true;
}

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
