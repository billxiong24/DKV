#ifndef KVPART_H
#define KVPART_H
#include "KVServer.h"
#include "KVSeed.h"


class KVPart : private KVServer {
    public:
        //calls KVServer constructor, assigns seeds to s
        KVPart(char *host, int port, char *rhost, int rport, std::vector<KVSeed> s) : KVServer(host, port, rhost, rport), seeds(s) {}

        void send_to_seeds();

    private:
        std::vector<KVSeed> seeds;
};


#endif
