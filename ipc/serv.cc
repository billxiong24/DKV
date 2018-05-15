#include "TCPServerWrapper.h"

int main(void) {
    TCPServerWrapper serv(7777);
    serv.start_server();
    return 0;
}
