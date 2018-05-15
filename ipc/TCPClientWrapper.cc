#include "TCPClientWrapper.h"
#define PACKET_TERM '\0'

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
    //char buffer[1024] = {0};
    //send(sock , hello , strlen(hello) , 0 );
    //valread = read( sock , buffer, 1024);
}

//void TCPClientWrapper::send_data(const void *buf, size_t len) {
    //if(send(this->get_read_fd(), buf, len, 0) < 0) {
        //throw "Connection closed unexpectedly.";
    //}
//}

//void TCPClientWrapper::read_data(std::function<void(TCPClientWrapper, std::string)> data_func) {
    //char buffer[1];
    //buffer[0] = 1;

    //std::string res;

    //while(buffer[0] != PACKET_TERM) {
        //if(read(this->get_read_fd(), buffer, sizeof(buffer)) < 0) {
            //close(this->get_read_fd());
            //throw "Failed to read from client.";
        //}

        //res += buffer[0];
    //}

    //cout << res << endl;

    ////data_func(*this, res);
//}
