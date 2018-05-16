#include "KVServer.h"
#include "../ipc/TCPClientWrapper.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

#define DELIM ':'

KVServer::KVServer(std::string host, int port) : host(string(host)), port(port) {
    //insert self into map
}

void KVServer::init(char *redis_host, int redis_port) {
    this->context = redisConnect(redis_host, redis_port);
    this->hash = this->init_hash(host, port);
    this->servers.insert(std::pair<size_t, Address>(this->hash, Address(host, port)));
}

size_t KVServer::get_hash() {
    return this->hash;
}

std::string KVServer::get_host() {
    return this->host;
}

int KVServer::get_port() {
    return this->port;
}

static std::vector<std::string> explode_str(char *str, char delim) {
    auto input = std::string(str);

    std::istringstream stream(input);
    std::string token;

    std::vector<std::string> arr;

    while(std::getline(stream, token, delim)) {
        arr.push_back(token);
    }

    return arr;
}

char *KVServer::serialize_info() {
    cout << this->port << endl;
    std::string str = std::string(this->host) + ":" + std::to_string(this->port);
    return &str[0];
}

KVServer KVServer::deserialize_info(char *str) {
    std::vector<std::string> info = explode_str(str, DELIM);
    return KVServer(info[0], std::stoi(info[1]));
}

void recv_func(TCPSocketWrapper serv, std::string res) {
    cout << "str:" << res << endl;
    //TODO once we receive a response from server, add this information to our map
    
    //serv.send_data("response\0", 10);
}

void KVServer::send_seed_func(char *host, int port) {

    TCPClientWrapper cli;
    //connect to seed
    cli.conn(host, port);
    //TODO send our host and port
    cli.send_data("erer\0", 5);
    //wait for information (list of servers in ring) from seed server
    bool res = cli.recv_data(recv_func);

    if(!res) {
        puts("Failed to receive data");
    }
}

void KVServer::bootstrap(std::vector<KVServer> seeds) {

    //store vectof of threads so we can join them all at the end
    vector<std::thread> threads;

    for(auto &server : seeds) {
        //using &str[0] converts from std::string to char *
        char *host = &(server.get_host())[0];
        int port = server.get_port();

        //create new thread to send info to each seed server
        std::thread conn([this, host, port] {
            this->send_seed_func(host, port);
        });

        threads.push_back(std::move(conn));
    }

    for(auto &t: threads) {
        t.join();
    }
}

void KVServer::listen() {

}

void KVServer::send_membership() {

}

std::string KVServer::get(char *key) {
    if(!key) {
        throw "key or value is null.";
    }
    else {
        redisReply *reply;
        //for some reason, must cast to redisReply, otherwise void ptr err
        reply = (redisReply *) redisCommand(this->context, "GET %s", key);
        //if reply is not a string value, which should never happen
        if(reply->type != REDIS_REPLY_STRING) {
            throw "value is not of type string.";
        }

        std::string value = std::string(reply->str);
        freeReplyObject(reply);

        return value;
    }
}

void KVServer::put(char *key, char *value) {
    if(!key || !value) {
        throw "key or value is null.";
        return;
    }

    void *reply = redisCommand(this->context, "SET %s %s", key, value);
    freeReplyObject(reply);
}

size_t KVServer::init_hash(std::string host, int port) {
    //concatenate host and port to use as hash string
    std::string hash_str = host + std::to_string(port);
    return gen_hash_key(hash_str);
}

size_t KVServer::gen_hash_key(std::string str) {
    std::hash<std::string> hash_func;
    return hash_func(str);
}

int main() {
    auto d = KVServer((char *) "127.0.0.1", 5656);
}
