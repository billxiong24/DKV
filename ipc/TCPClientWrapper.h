#ifndef TCPCLIENTWRAPPER_H
#define TCPCLIENTWRAPPER_H
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include "TCPSocketWrapper.h"

class TCPClientWrapper : public TCPSocketWrapper {
    public:
        TCPClientWrapper();
        void conn(char *host, int port);
        std::string recv_data();
        //void send_data(const void *buf, size_t len);

    private:
        int sockfd;

};

#endif
