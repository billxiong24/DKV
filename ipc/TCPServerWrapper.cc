#include "TCPServerWrapper.h"

#define BACKLOG 20

TCPServerWrapper::TCPServerWrapper(int port) : fd(-1), port(port) {
}

void TCPServerWrapper::start_server() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    //char buffer[1024] = {0};
    //char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
    }

    // Forcefully attaching socket to the port 8080
    //use true in the socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                &opt, sizeof(opt)))
    {
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(server_fd, (struct sockaddr *)&address, 
                sizeof(address))<0)
    {
    }
    if (listen(server_fd, BACKLOG) < 0) {
    }

    int new_socket = 0;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                    (socklen_t*)&addrlen))<0)
    {
    }
    //valread = read( new_socket , buffer, 1024);
    //printf("%s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 );
}
