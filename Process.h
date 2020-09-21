//
// Created by daimiaopeng on 2020/9/20.
//

#ifndef WUZIQI_SERVER_PROCESS_H
#define WUZIQI_SERVER_PROCESS_H

#include <memory>
#include "protocol/Message.h"
#include "Session.h"
#include "Redis.h"

using namespace std;


class Process {
public:
    Process(shared_ptr<Session> session, shared_ptr<char[]> buff, int len, Redis &redis) : _session(session),
                                                                                           _buff(buff), _len(len),
                                                                                           _redis(redis) {
        cmd c;
        c.ParseFromArray(buff.get(), len);
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
            default:
                LOG(INFO) << "cmd default";
        }
    }


private:
    shared_ptr<Session> _session;
    shared_ptr<char[]> _buff;
    Redis &_redis;
    int _len;

private:
    void cmd0();

    void cmd2();

    void cmd4();

};


#endif //WUZIQI_SERVER_PROCESS_H