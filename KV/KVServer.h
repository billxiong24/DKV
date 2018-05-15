#ifndef KVSERVER_H
#define KVSERVER_H

#include <string>
#include <vector>
#include <map>
#include <hiredis/hiredis.h>
using namespace std;

class KVServer {

    public:
        KVServer(char *host, int port, char *redis_host, int redis_port);
        size_t get_hash();
        string get(char *key);
        void put(char *key, char *value);
        std::string get_host();
        int get_port();
        void listen();
        void send_membership();

    private:
        size_t hash;
        redisContext *context;
        std::map<size_t, KVServer> servers;
        std::string host;
        int port;
        size_t init_hash(char *host, int port);

        static size_t gen_hash_key(string str);


};


#endif
