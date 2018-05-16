#include "TCPClientWrapper.h"

#include <iostream>
using namespace std;

TCPClientWrapper::TCPClientWrapper() : TCPSocketWrapper() {
}


void TCPClientWrapper::conn(char *host, int port) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    int new_fd;

    if ((new_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw "Socket creation error.";
    }
    this->set_read_fd(new_fd);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(this->get_read_fd(), (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        throw "Connection error.";
    }
}

bool TCPClientWrapper::recv_data(std::function<void(TCPSocketWrapper, std::string)> data_func) {
    return this->read_data(this->get_read_fd(), data_func);
}
