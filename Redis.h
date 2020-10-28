//
// Created by daimiaopeng on 2020/9/21.
//

#ifndef WUZIQI_SERVER_REDIS_H
#define WUZIQI_SERVER_REDIS_H


#include <hiredis/hiredis.h>
#include <iostream>
#include <boost/regex.hpp>
#include <memory>
#include <set>
#include <glog/logging.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "tool/md5.h"

using namespace std;

class Redis {
private:
    string ip;
    int port;
    redisContext *conn;

public:
    Redis(string ip, int port, string passwd);

    Redis(string ip, int port);

    void init();

    void pushMessageQueue(const string &message);

    string getName(int fd);

    string getName(const string &token);

    int getFd(const string &name);

    void setName(int fd, const string &name);

    void delName(int fd);

    std::set<string> getOnile();

    redisContext *getConn() { return conn; };


    string login(const string &name, const string &passwd, string &message);

    bool registered(const string &name, const string &passwd, string &message);

    int lenMessage();

    string popMessageQueue();

    int getRegisterNums();

    void clearToken();

    void setFd(int fd, const string &name);

    void setToken(const string &token, const string &name);

private:
    auto redisReply_ptr(void *reply);

};


#endif //WUZIQI_SERVER_REDIS_H
