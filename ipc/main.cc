#include "TCPClientWrapper.h"
#include <iostream>
#include <thread>
using namespace std;
void data_func(TCPSocketWrapper serv, std::string res) {
    cout << "str:" << res << endl;
    
    //serv.send_data("response\0", 10);
}

void thread_func(char *host, int port) {
    TCPClientWrapper cli;
    cli.conn(host, port);
    while(true) {
        cli.send_data("erer\0", 5);
        bool res = cli.recv_data(data_func);
        if(!res) {
            puts("cant receive data");
            break;
        }
        sleep(2);
    }
}

int main(int argc, const char *argv[]) {

    char *host = (char *) "127.0.0.1";
    std::thread first(thread_func, host, 7777);
    //std::thread sec(thread_func, host, 7778);


    first.detach();
    //sec.detach();

    while(true);

    return 0;
}
