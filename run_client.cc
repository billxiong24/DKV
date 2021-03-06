#include "KV/KVServer.h"
#include "address/Address.h"
#include <vector>

int main(void) {

    char *host = (char *) "127.0.0.1";


    KVServer s1 = KVServer(std::string(host), 5656);
    KVServer *ptr = &s1;

    std::thread listen([ptr] {
        ptr->listen();
    });

    std::vector<Address> addr;
    addr.push_back(Address(std::string(host), 7777));
    //addr.push_back(Address(std::string(host), 7778));

    s1.bootstrap(addr);


    while(true);

    
    return 0;
}
