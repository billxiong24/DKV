#ifndef KVSEED_H
#define KVSEED_H
#include "KVServer.h"

class KVSeed : private KVServer {

    public:
        KVSeed(char *host, int port, char *rhost, int rport) : KVServer(host, port, rhost, rport) {
        
        }
};


#endif
