//
// Created by daimiaopeng on 2020/9/21.
//

#ifndef WUZIQI_SERVER_REDIS_H
#define WUZIQI_SERVER_REDIS_H


#include <hiredis/hiredis.h>
#include "glog/logging.h"
#include <iostream>
#include <boost/regex.hpp>
#include <memory>
#include <set>

using namespace std;

class Redis {
private:
    string ip;
    u_int port;
    redisContext *conn;

public:
    Redis(string ip, u_int port, string passwd);

    Redis(string ip, u_int port);

    void init();

    void pushMessageQueue(const string &message);

    string getName(int fd);

    string getName(const string &token);

    int getFd(const string &name);

    void setName(int fd, const string &name);

    void delName(int fd);

    set<string> geOnile();

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
