#ifndef KVSERVER_H
#define KVSERVER_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <hiredis/hiredis.h>
#include "../address/Address.h"
#include "../ipc/TCPClientWrapper.h"
#include "../ipc/TCPServerWrapper.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
using namespace std;

class KVServer {

    public:
        KVServer(std::string host, int port);

        void init(char *redis_host, int redis_port);
        size_t get_hash();
        string get(char *key);
        void put(char *key, char *value);
        std::string get_host();
        int get_port();
        void listen();
        void send_membership();
        std::string serialize_map();

        void bootstrap(std::vector<Address> seeds);
        void map_ins(size_t, Address);
        Address map_get(size_t);

        ~KVServer();

    private:
        std::mutex *map_lock;
        size_t hash;
        redisContext *context;
        std::map<size_t, Address> servers;

        std::string host;
        int port;
        void recv_func(std::string res);
        //static size_t init_hash(std::string host, int port);

        //static size_t gen_hash_key(string str);

        void send_seed_func(char *host, int port);

        std::string serialize_info();
        //Address deserialize_info(std::string str);
};


#endif
