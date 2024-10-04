#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sstream>
#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include "managers/user/userManager.h"
#include "managers/shop/shopManager.h"
#include "managers/admin/adminManager.h"
#include "managers/deliver/deliverManager.h"
#include "con2redis/src/con2redis.h"
#include <thread>
#include <typeinfo>


class HttpServer {
public:
    HttpServer(int port) : port_(port) {}

    void start();
    void stop();

private:
    int port_;
    void handleClient(int clientSocket);
};




#endif // HTTP_SERVER_H
