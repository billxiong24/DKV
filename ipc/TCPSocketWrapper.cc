#include "TCPSocketWrapper.h"
using namespace std;
#define PACKET_TERM '\0'

TCPSocketWrapper::TCPSocketWrapper() : read_fd(-1) {

}

int TCPSocketWrapper::get_read_fd() {
    return this->read_fd;
}

void TCPSocketWrapper::set_read_fd(int fd) {
    this->read_fd = fd;
}

std::string TCPSocketWrapper::read_data(int read_fd) {

    char buffer[1];
    buffer[0] = -1;

    std::string res;

    while(buffer[0] != PACKET_TERM) {
        int num_read = read(read_fd, buffer, sizeof(buffer));
        if(num_read < 0) {
            close(read_fd);
            //puts("exception throwing");
            throw "Failed to read from client.";
        }
        else if(num_read == 0) {
            close(read_fd);
            res.clear();
            return res;
        }
        res += buffer[0];
    }

    //data_func(*this, res);
    return res;
}

void TCPSocketWrapper::send_data(const void *buf, size_t len) {
    if(send(this->read_fd, buf, len, 0) < 0) {
        throw "Connection closed unexpectedly.";
    }
}
