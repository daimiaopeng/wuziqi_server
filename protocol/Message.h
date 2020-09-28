#ifndef WUZIQI_SERVER_MESSAGE_H
#define WUZIQI_SERVER_MESSAGE_H

#include "glog/logging.h"
#include "base.pb.h"


struct message {
    u_int len;
    char data[0];
};


#endif //WUZIQI_SERVER_MESSAGE_H
