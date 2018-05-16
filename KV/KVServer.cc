#include "KVServer.h"

#define DELIM ':'
#define ARR_DELIM '$'
size_t gen_hash(std::string host, int port);
static std::string serialize(std::string host, int port); 

KVServer::KVServer(std::string host, int port) : host(host), port(port) {
    this->map_lock = new std::mutex();
}

void KVServer::init(char *redis_host, int redis_port) {
    //this->context = redisConnect(redis_host, redis_port);
    this->hash = gen_hash(host, port);
    //insert self into map
    this->map_ins(this->hash, Address(host, port));
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

static std::vector<std::string> explode_str(std::string input, char delim) {

    std::istringstream stream(input);
    std::string token;

    std::vector<std::string> arr;

    while(std::getline(stream, token, delim)) {
        arr.push_back(token);
    }

    return arr;
}

static std::string serialize(std::string host, int port) {
    std::string str = std::string(host) + ":" + std::to_string(port);
    return str;
}

std::string KVServer::serialize_info() {
    return serialize(this->host, this->port);
}


std::string KVServer::serialize_map() {

    std::string ret;

    map_lock->lock();
    for(auto &pair : this->servers) {
        Address addr = pair.second;
        std::string addr_str = serialize(addr.host, addr.port);
        ret += addr_str;
        ret += ARR_DELIM;
    }
    //string will have an extra delimiter, remove it
    map_lock->unlock();
    ret.pop_back();
    ret += '\0';

    return ret;
}

static Address deserialize_info(std::string str) {
    std::vector<std::string> info = explode_str(str, DELIM);
    return Address(info[0], std::stoi(info[1]));
}


void KVServer::recv_func(std::string res) {
    //TODO once we receive a response from server, add this information to our map
    
    std::vector<std::string> addresses = explode_str(res, ARR_DELIM);

    //loop through addresses and split into ip and port
    for(auto &str_addr: addresses) {
        Address addr = deserialize_info(str_addr);
        size_t addr_hash = gen_hash(addr.host, addr.port);
        //add server's address to our treemap of known addresses
        this->servers.insert(std::pair<size_t, Address>(addr_hash, addr));
    }

    //serv.send_data("response\0", 10);
}

void KVServer::send_seed_func(char *host, int port) {

    TCPClientWrapper cli;
    //connect to seed
    try {
        cli.conn(host, port);
    }
    catch(const char *msg) {
        cout << msg << endl;
        //couldn't connect to seed, timeout and wait again
        sleep(5);
        this->send_seed_func(host, port);
        return;
    }

    //TODO send our host and port
    std::string temp = this->serialize_info();

    char *serial = &temp[0];

    cli.send_data(serial, strlen(serial) + 1);
    //wait for information (list of servers in ring) from seed server
    std::string res;

    try {
        res = cli.recv_data();
    }
    catch(const char *msg) {
        cout << msg << endl;
        return;
    }

    if(res.size() == 0) {
        puts("Failed to receive data");
        return;
    }

    this->recv_func(res);
}

void KVServer::bootstrap(std::vector<Address> seeds) {

    //store vectof of threads so we can join them all at the end
    vector<std::thread> threads;

    for(auto &server : seeds) {
        //using &str[0] converts from std::string to char *
        char *host = &(server.host)[0];
        int port = server.port;


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

void server_func(TCPSocketWrapper server, void *arg, std::string res) {
    //if we receive empty packet, discard it
    if(res.size() == 0) {
        return;
    }

    //super ratchet but watever
    KVServer *kvs = (KVServer *) arg;

    Address deserial = deserialize_info(res);
    size_t hash = gen_hash(deserial.host, deserial.port);
    kvs->map_ins(hash, deserial);

    Address test = kvs->map_get(hash);

    //TODO send our map of addresses back to client
    std::string serial_map = kvs->serialize_map();


    //send client our map of addresses
    server.send_data(&serial_map[0], serial_map.size());
}

void KVServer::listen() {
    TCPServerWrapper server(this->port);
    server.start_server();

    void *arg = (void *) this;

    server.recv_data(arg, server_func);
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

size_t gen_hash(std::string host, int port) {
    //concatenate host and port to use as hash string
    std::string hash_str = host + std::to_string(port);
    std::hash<std::string> hash_func;
    return hash_func(hash_str);
}

void KVServer::map_ins(size_t key, Address addr) {
    map_lock->lock();
    this->servers.insert(std::pair<size_t, Address>(key, addr));
    map_lock->unlock();
}

Address KVServer::map_get(size_t key) {
    map_lock->lock();
    std::map<size_t, Address>::iterator it = this->servers.find(key);
    map_lock->unlock();

    //it->first is key, it->second is value
    return it->second;
}

KVServer::~KVServer() {
    delete this->map_lock;
}

//void test_func(void *arg) {

    //KVServer *test = (KVServer *) arg;

    //cout << test->get_host() << endl;
//}

//int main() {
    //auto d = KVServer((char *) "127.0.0.1", 5656);

    //test_func((void *) &d);
//}
