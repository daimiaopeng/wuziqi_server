//
// Created by daimiaopeng on 2020/9/20.
//

#ifndef WUZIQI_SERVER_PROCESS_H
#define WUZIQI_SERVER_PROCESS_H

#include "protocol/Message.h"
#include <memory>

using namespace std;

class Session;

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
            default:
                LOG(INFO) << "cmd default";
        }
    }


private:
    shared_ptr<Session> _session;
    shared_ptr<char[]> _buff;
    int _len;
private:
    void cmd0() {
        test t;
        t.ParseFromArray(_buff.get(), _len);
        LOG(INFO) << "cmd 0 string: " << t.inform();

    }
};


#endif //WUZIQI_SERVER_PROCESS_H
