#include "TCPSocketWrapper.h"

TCPSocketWrapper::TCPSocketWrapper() : read_fd(-1) {

}

int TCPSocketWrapper::get_read_fd() {
    return this->read_fd;
}

void TCPSocketWrapper::set_read_fd(int fd) {
    this->read_fd = fd;
}

void TCPSocketWrapper::read_data(std::function<void(TCPSocketWrapper, std::string)> data_func) {
    char buffer[1];
    buffer[0] = 1;

    std::string res;

    while(buffer[0] != PACKET_TERM) {
        if(read(this->read_fd, buffer, sizeof(buffer)) < 0) {
            close(this->read_fd);
            throw "Failed to read from client.";
        }

        res += buffer[0];
    }

    data_func(*this, res);
}

void TCPSocketWrapper::send_data(const void *buf, size_t len) {
    if(send(this->read_fd, buf, len, 0) < 0) {
        throw "Connection closed unexpectedly.";
    }
}
