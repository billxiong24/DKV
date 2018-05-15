#ifndef TCPCLIENTWRAPPER_H
#define TCPCLIENTWRAPPER_H
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

class TCPClientWrapper {
    public:
        TCPClientWrapper();
        void conn(char *host, int port);
        void send_data(const void *buf, size_t len);
        void read_data();

    private:
        int sockfd;

};

#endif
