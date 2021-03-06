#include "TCPServerWrapper.h"

#define BACKLOG 20

TCPServerWrapper::TCPServerWrapper(int port) : TCPSocketWrapper(), fd(-1), port(port) {
    memset(&address, 0, sizeof(address));
}

void TCPServerWrapper::start_server() {
    int addrlen = sizeof(address);

    if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {

    }

    int server_fd = this->fd;

    //use true in the socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {

    }

    if (listen(server_fd, BACKLOG) < 0) {

    }
}

void TCPServerWrapper::recv_data(void *arg, std::function<void(TCPSocketWrapper, void *, std::string)> data_func) {
    int addrlen = sizeof(address);
    int new_fd;
    while(true) {
        if ((new_fd = accept(this->fd, (struct sockaddr *)&address, 
                        (socklen_t*)&addrlen))<0) {
            return;
        }

        //[] include capture variables to include in the lambda
        std::thread conn([this, new_fd, data_func, arg] { 
            while(true) {
                this->set_read_fd(new_fd);

                if(this->get_read_fd() < 0) {
                    throw "Invalid client file descriptor.";
                }

                //if client closes connection, catch exception and exit thread
                try {
                    this->read_cb(new_fd, arg, data_func);
                }
                catch(char const*msg) {
                    break;
                }
            }
        });

        conn.detach();
    }
}
