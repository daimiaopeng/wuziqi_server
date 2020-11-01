//
// Created by daimiaopeng on 2020/10/14.
//

#ifndef WUZIQI_SERVER_DATABASE_H
#define WUZIQI_SERVER_DATABASE_H

//#include "protocol/_database.h"
#include <sqlite_orm.h>
#include <string>
#include <boost/regex.hpp>
#include <memory>
#include <set>
#include <glog/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "tool/md5.h"
#include <boost/date_time/posix_time/posix_time.hpp>

#define BOOST_DATE_TIME_SOURCE

using namespace std;
using namespace sqlite_orm;

struct UserGameInfor {
    int id; // id
    string name; // 昵称
    int integral; // 积分
    int level; //级别
    int numsGame; // 总盘数
    int win; //赢
    int lose; //输
    int draw; //平
    int gameCurrency; //游戏币
    string avatar; //头像
};
struct User {
    int id; // id
    string name; //账户
    string passwd; //密码
    string nicheng; //昵称
    string email; //邮件
    string touxiang; //头像
};

struct SignInLog {
    int id; // id
    string name; //账户
    string time; //时间
};

inline auto InitStorage(const string &path) {
    return make_storage(
            "wuziqi.db",
            make_table(
                    "UserGameInfor",
                    make_column("id", &UserGameInfor::id, autoincrement(), primary_key()),
                    make_column("name", &UserGameInfor::name),
                    make_column("integral", &UserGameInfor::integral),
                    make_column("level", &UserGameInfor::level),
                    make_column("numsGame", &UserGameInfor::numsGame),
                    make_column("win", &UserGameInfor::win),
                    make_column("lose", &UserGameInfor::lose),
                    make_column("draw", &UserGameInfor::draw),
                    make_column("gameCurrency", &UserGameInfor::gameCurrency),
                    make_column("avatar", &UserGameInfor::avatar)),
            make_table(
                    "User",
                    make_column("id", &User::id, autoincrement(), primary_key()),
                    make_column("name", &User::name),
                    make_column("passwd", &User::passwd),
                    make_column("nicheng", &User::nicheng),
                    make_column("email", &User::email),
                    make_column("touxiang", &User::touxiang)),
            make_table(
                    "SignInLog",
                    make_column("id", &SignInLog::id, autoincrement(), primary_key()),
                    make_column("name", &SignInLog::name),
                    make_column("time", &SignInLog::time))
    );
}


using Storage = decltype(InitStorage(""));

class Database {
public:
    Database() {
        _storage.sync_schema();
    };

    void initUserGameInfor(const string &name);

    bool registered(const string &name, const string &passwd, const string &nicheng, const string &email,
                    const string &touxiang, string &message);

    bool login(const string &name, const string &passwd, string &message);

    UserGameInfor getUserGameInfor(const string &name);

    User getUserInfor(const string &name);

    void winGame(const string &name);

    void loseGame(const string &name);

    void drawGame(const string &name);

    void signInLog(const string &name);
public:
    Storage _storage = InitStorage("db.sqlite");
};


#endif //WUZIQI_SERVER_DATABASE_H
