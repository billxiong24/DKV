#include "KV/KVServer.h"
#include <thread>




int main(int argc, char **argv) {

    char *host = (char *) "127.0.0.1";

    //KVServer s1 = KVServer(host, 5656);

    KVServer seed = KVServer(host, atoi(argv[1]));
    KVServer *ptr = &seed;
    seed.init(host, 6379);

    std::thread listen([ptr] {
        ptr->listen();
    });

    cout << "hello" << endl;


    listen.detach();

    puts("hello world");

    while(true);
    return 0;
}
