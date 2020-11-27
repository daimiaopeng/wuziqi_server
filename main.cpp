#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <memory>
#include "Server.h"

using namespace std;
using ptree = boost::property_tree::ptree;

//初始化日志服务
void initLog(bool isWriteLog, string logDir) {
    google::InitGoogleLogging("serverLog");    //初始化log的名字为serverLog
    if (isWriteLog == true) {
        google::SetLogDestination(google::GLOG_INFO, logDir.c_str());    //设置输出日志的文件夹，文件夹必须已经存在
    }
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogFilenameExtension("log_");
    FLAGS_colorlogtostderr = true;  // 设置日志颜色
    FLAGS_logbufsecs = 1;
    FLAGS_max_log_size = 1024;
    FLAGS_stop_logging_if_full_disk = true;
}


int main(int argc, char *argv[]) {
    boost::property_tree::ptree m_pt;
    boost::property_tree::ini_parser::read_ini("config.ini", m_pt);
    string ip = m_pt.get<string>("server.ip", "127.0.0.1");
    int port = m_pt.get<int>("server.port", 6379);
    bool isWriteLog = m_pt.get<bool>("server.isWriteLog", true);
    string logDir = m_pt.get<string>("server.logDir", "./log");
    initLog(isWriteLog, logDir);
    LOG(INFO) << "Server is running on ip:" << ip << " port:" << port << " isWriteLog:" << isWriteLog << " logDir:"
              << logDir;
    boost::asio::io_context ioContext;
    auto server(make_shared<Server>(ioContext, port));
    server->run();
    ioContext.run();
    return 0;
}