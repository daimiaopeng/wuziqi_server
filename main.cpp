#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <memory>
#include "Server.h"

using namespace std;
using ptree = boost::property_tree::ptree;

//初始化日志服务
void initLog(bool isWriteLog) {
    google::InitGoogleLogging("serverLog");    //初始化log的名字为serverLog
    if (isWriteLog) {
        google::SetLogDestination(google::GLOG_INFO, "../log/");    //设置输出日志的文件夹，文件夹必须已经存在
    }
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogFilenameExtension("log_");
    FLAGS_colorlogtostderr = true;  // Set log color
    FLAGS_logbufsecs = 0;  // Set log output speed(s)
    FLAGS_max_log_size = 1024;  // Set max log file size
    FLAGS_stop_logging_if_full_disk = true;  // If disk is ful
}


int main(int argc, char *argv[]) {
    boost::property_tree::ptree m_pt;
    boost::property_tree::ini_parser::read_ini("config.ini", m_pt);
    string ip = m_pt.get<string>("server.ip", "127.0.0.1");
    int port = m_pt.get<int>("server.port", 6379);
    bool isWriteLog = m_pt.get<bool>("isWriteLog", false);
    initLog(isWriteLog);
    LOG(INFO) << "Server is running on ip:" << ip << " port:" << port << " isWriteLog:" << isWriteLog;
    boost::asio::io_context ioContext;
    auto server(make_shared<Server>(ioContext, port));
    server->run();
    ioContext.run();
    return 0;
}