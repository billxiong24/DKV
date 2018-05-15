#ifndef TCPSERVERWRAPPER_H
#define TCPSERVERWRAPPER_H
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <functional>
#include <thread>
#include "TCPSocketWrapper.h"

class TCPServerWrapper : public TCPSocketWrapper {

    public:
        TCPServerWrapper(int port);

        void start_server();
        void recv_data(std::function<void(TCPSocketWrapper, std::string)> data_func);

    private:
        int port;
        int fd;
        struct sockaddr_in address;

        //static void thread_func(TCPServerWrapper serv, int fd, std::function<void(TCPSocketWrapper, std::string)> data_func);

};



#endif
