#include "TCPClientWrapper.h"

TCPClientWrapper::TCPClientWrapper() {
    this->sockfd = -1;
}


void TCPClientWrapper::conn(char *host, int port) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw "Socket creation error.";
    }


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(this->sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        throw "Connection error.";
    }
    //char buffer[1024] = {0};
    //send(sock , hello , strlen(hello) , 0 );
    //valread = read( sock , buffer, 1024);
}

void TCPClientWrapper::send_data(const void *buf, size_t len) {
    if(this->sockfd < 0) {
        throw "Socket descriptor invalid.";
    }

    if(send(this->sockfd, buf, len, 0) < 0) {
        throw "Connection closed unexpectedly.";
    }
}

void TCPClientWrapper::read_data() {

}
