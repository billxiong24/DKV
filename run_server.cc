#include "KV/KVServer.h"


int main(int argc, char **argv) {

    char *host = (char *) "127.0.0.1";

    //KVServer s1 = KVServer(host, 5656);

    KVServer seed = KVServer(host, atoi(argv[1]));
    seed.init(host, 6379);

    seed.listen();
    
    return 0;
}
