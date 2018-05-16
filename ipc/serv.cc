#include "TCPServerWrapper.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

void data_func(TCPSocketWrapper serv, void *arg, std::string res) {
    if(res.size() == 0)
        return;

    cout << res << endl;
    serv.send_data("response\0", 9);
}

int main(int argc, const char *argv[]) {
    TCPServerWrapper serv(atoi(argv[1]));
    serv.start_server();

    char *str = (char *) malloc(10);
    
    serv.recv_data(NULL, data_func);
    return 0;
}
