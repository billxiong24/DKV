#include "TCPClientWrapper.h"

int main() {
    TCPClientWrapper cli;
    cli.conn((char *) "127.0.0.1", 7777);
    //cli.read_data();

    
    return 0;
}
