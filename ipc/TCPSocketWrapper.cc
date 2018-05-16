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
std::string TCPSocketWrapper::parse_sock_data(int read_fd) {
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

    return res;
}

std::string TCPSocketWrapper::read_data(int read_fd) {
    return this->parse_sock_data(read_fd);
}

//if something wrong with read, will send back an empty string, cuz we cant do null for some reason...
void TCPSocketWrapper::read_cb(int read_fd, void *arg, std::function<void(TCPSocketWrapper, void *arg, std::string)> data_func) {

    std::string res = this->parse_sock_data(read_fd);
    data_func(*this, arg, res);
}

void TCPSocketWrapper::send_data(const void *buf, size_t len) {
    if(send(this->read_fd, buf, len, 0) < 0) {
        throw "Connection closed unexpectedly.";
    }
}
