#include "Server.h"
#include "glog/logging.h"


class Session;

class Server;

void initLog() {
    google::InitGoogleLogging("serverLog");    //初始化log的名字为daqing
//    google::SetLogDestination(google::GLOG_INFO, "../log/");    //设置输出日志的文件夹，文件夹必须已经存在
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetLogFilenameExtension("log_");
    FLAGS_colorlogtostderr = true;  // Set log color
    FLAGS_logbufsecs = 0;  // Set log output speed(s)
    FLAGS_max_log_size = 1024;  // Set max log file size
    FLAGS_stop_logging_if_full_disk = true;  // If disk is ful
}


int main(int argc, char *argv[]) {

    initLog();

    boost::asio::io_context ioContext;

    Server server(ioContext, std::atoi("9123"));

//    std::thread ioThread([&]() { ioContext.run(); });
//    ioThread.detach();
//    while (1) {
////        cout<<"size"<<cilentList.size()<<endl;
//        std::this_thread::sleep_for(chrono::seconds(10000));
//    }
    ioContext.run();

    return 0;
}