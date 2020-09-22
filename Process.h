//
// Created by daimiaopeng on 2020/9/20.
//

#ifndef WUZIQI_SERVER_PROCESS_H
#define WUZIQI_SERVER_PROCESS_H

#include <memory>
#include <mutex>
#include "protocol/Message.h"
#include "Session.h"
#include "Redis.h"
#include "Server.h"

using namespace std;

class Process {
public:
    Process(shared_ptr<Session> session, shared_ptr<char[]> buff, int len) : _session(session), _buff(buff), _len(len) {
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
            case 6:
                cmd6();
                break;
            case 8:
                cmd8();
                break;
            case 11:
                cmd11();
                break;
            default:
                LOG(INFO) << "cmd default";
        }
    }


private:
    shared_ptr<Session> _session;
    shared_ptr<char[]> _buff;
    int _len;

private:
    void cmd0();

    void cmd2();

    void cmd4();

    void cmd6();

    void cmd8();

    void cmd11();
};


#endif //WUZIQI_SERVER_PROCESS_H