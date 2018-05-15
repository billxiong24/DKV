#include "KVServer.h"
#include <iostream>
#include <vector>

KVServer::KVServer(char *host, int port, char *redis_host, int redis_port) : host(string(host)) {
    //this->context = redisConnect(redis_host, redis_port);
    this->hash = this->init_hash(host, port);
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

size_t KVServer::init_hash(char *host, int port) {
    std::string temp = string(host);
    //concatenate host and port to use as hash string
    std::string hash_str = host + std::to_string(port);

    return gen_hash_key(hash_str);
}

size_t KVServer::gen_hash_key(std::string str) {
    std::hash<std::string> hash_func;
    return hash_func(str);
}
