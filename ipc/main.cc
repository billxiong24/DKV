#include "TCPClientWrapper.h"
#include <iostream>
#include <thread>
#include <vector>
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
        std::string res = cli.recv_data();
        cout << res << endl;
        if(res.empty()) {
            puts("cant receive data");
            break;
        }
        sleep(2);
    }
}

int main(int argc, const char *argv[]) {

    char *host = (char *) "127.0.0.1";

    vector<std::thread> threads;

    for (int i = 0; i < 2; i++) {
        std::thread first(thread_func, host, 7777 + i);
        threads.push_back(std::move(first));
    }

    for(auto &t : threads) {
        t.join();
    }



    //while(true);

    return 0;
}
