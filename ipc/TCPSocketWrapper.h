#ifndef TCPSOCKETWRAPPER_H
#define TCPSOCKETWRAPPER_H
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <iostream>

#define PACKET_TERM '\0'

class TCPSocketWrapper {

    public:
        TCPSocketWrapper();
        void send_data(const void *buf, size_t len);
        std::string read_data(int read_fd);

    protected:
        int get_read_fd();
        void set_read_fd(int fd);

    private:
        int read_fd;


};


#endif
