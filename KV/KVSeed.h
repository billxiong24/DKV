#ifndef KVSEED_H
#define KVSEED_H
#include "KVServer.h"

class KVSeed : private KVServer {

    public:
        KVSeed(char *host, int port) : KVServer(host, port) {
        
        }
};


#endif
