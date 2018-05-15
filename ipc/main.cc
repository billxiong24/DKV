#include "TCPClientWrapper.h"
#include <iostream>
using namespace std;
void data_func(TCPSocketWrapper serv, std::string res) {
    cout << res << endl;
    //serv.send_data("response\0", 10);
}

int main(int argc, const char *argv[]) {
    TCPClientWrapper cli, cli2;
    cli.conn((char *) "127.0.0.1", 7777);
    cli2.conn((char *) "127.0.0.1", 7778);
    cli.send_data("erer\0", 5);
    cli2.send_data("herd\0", 5);

    cli.read_data(data_func);
    cli2.read_data(data_func);
    
    return 0;
}
