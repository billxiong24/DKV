#ifndef KVSERVER_H
#define KVSERVER_H

#include <string>
#include <vector>
#include <map>
#include <hiredis/hiredis.h>
#include "../address/Address.h"
using namespace std;

class KVServer {

    public:
        KVServer(std::string host, int port);
        KVServer deserialize_info(char *str);

        void init(char *redis_host, int redis_port);
        size_t get_hash();
        string get(char *key);
        void put(char *key, char *value);
        std::string get_host();
        int get_port();
        void listen();
        void send_membership();

        void bootstrap(std::vector<KVServer> seeds);

    private:
        size_t hash;
        redisContext *context;
        std::map<size_t, Address> servers;
        std::string host;
        int port;
        size_t init_hash(std::string host, int port);

        static size_t gen_hash_key(string str);

        void send_seed_func(char *host, int port);

        char *serialize_info();


};


#endif
