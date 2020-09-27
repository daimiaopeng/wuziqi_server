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
    Process(shared_ptr<Session> session, shared_ptr<char[]> buff, int len) : _session(session), _buff(buff),
                                                                             _len(len) {}

    void resolve();

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

    void cmd13();
};


#endif //WUZIQI_SERVER_PROCESS_H