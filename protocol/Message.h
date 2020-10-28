#ifndef WUZIQI_SERVER_MESSAGE_H
#define WUZIQI_SERVER_MESSAGE_H


#ifdef _WIN32
#include "windows/base.pb.h"
#else

#include "linux/base.pb.h"

#endif

#include "glog/logging.h"

struct message {
    int len;
    char data[1];
};


#endif //WUZIQI_SERVER_MESSAGE_H
