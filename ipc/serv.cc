#include "TCPServerWrapper.h"
#include <iostream>
using namespace std;

void data_func(TCPSocketWrapper serv, std::string res) {

    cout << res << endl;
    serv.send_data("response\0", 9);
}

int main(int argc, const char *argv[]) {
    TCPServerWrapper serv(atoi(argv[1]));
    serv.start_server();
    serv.recv_data(data_func);
    return 0;
}
