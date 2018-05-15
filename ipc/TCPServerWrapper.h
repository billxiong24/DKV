#ifndef TCPSERVERWRAPPER_H
#define TCPSERVERWRAPPER_H
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

class TCPServerWrapper {

    public:
        TCPServerWrapper(int port);

        void start_server();

    private:
        int port;
        int fd;


};



#endif
