#include "KVServer.h"

#define READ "R"
#define WRITE "W"
#define READ_REPLICATE "RREP"
#define WRITE_REPLICATE "WREP"

size_t gen_hash(std::string host, int port);

KVServer::KVServer(std::string host, int port) : host(host), port(port) {
    //this->map_lock = new std::mutex();
    this->ring = new KVRing<Address>(1);
}

void KVServer::init(char *redis_host, int redis_port) {
    //this->context = redisConnect(redis_host, redis_port);
    this->hash = gen_hash(host, port);
    //insert self into map
    this->map_ins(this->hash, Address(host, port));
}

const std::map<size_t, Address>& KVServer::get_servers() {
    return this->ring->get_map();
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

std::string KVServer::serialize_info() {
    return this->serializer.serialize_addr(this->host, this->port);
}

std::string KVServer::serialize_map() {

    std::string ret;
    //map_lock->lock();
    ret = this->serializer.serialize_map(this->servers);
    //map_lock->unlock();

    return ret;
}

void KVServer::recv_func(std::string res) {
    //TODO once we receive a response from server, add this information to our map
    
    std::vector<Address> addrs = this->serializer.deserialize_addr_arr(res);

    //loop through addresses and split into ip and port
    for(auto &addr: addrs) {
        size_t addr_hash = gen_hash(addr.host, addr.port);
        //syncrhonously add server's address to our treemap of known addresses
        this->map_ins(addr_hash, addr);
    }
    
    //map_lock->lock();
    //for(auto &pair : this->servers) {
        //cout << pair.first << endl;
        //cout << pair.second.host << endl;
        //cout << pair.second.port << endl;
    //}

    //cout << "--------------" << endl;
    //map_lock->unlock();
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

    
    puts(serial);
    cli.send_data(serial, strlen(serial) + 1);
    puts("received data in seed");
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
        puts("looping through seeds");
        std::thread conn([this, host, port] {
            this->send_seed_func(host, port);
        });
        threads.push_back(std::move(conn));
    }

    for(auto &t: threads) {
        t.join();
    }
}

bool starts_with(std::string str, std::string prefix) {

    //convert str to uppercase
    for (auto & c: str) {
        c = toupper(c);
    }

    return !str.compare(0, prefix.size(), prefix);
}

//this function handles all received packets (boostrap, read req, write req, etc.)
void server_func(TCPSocketWrapper server, void *arg, std::string res) {
    //if we receive empty packet, discard it
    if(res.size() == 0) {
        return;
    }

    //handle read request
    if(starts_with(res, READ)) {
        //TODO check key's position on hashing ring
        //if not us, then forward to machine who can serve read request.
        //else respond with data
    
    }
    //handle write requst
    else if(starts_with(res, WRITE)) {
        //TODO check position on hashing ring
        //check preference list, forward the request to those nodes
    
    }
    else if(starts_with(res, WRITE_REPLICATE)) {
        //TODO this is when a coordinator asks this machine to replicate a write request.
        //write to redis, and reply back to coordinator. (sloppy quorum)
    
    }
    //client send its information for us to store in our map
    else {
        puts("im here received anon packet");
        //super ratchet but watever
        KVServer *kvs = (KVServer *) arg;

        Address deserial = kvs->serializer.deserialize_addr(res);

        size_t hash = gen_hash(deserial.host, deserial.port);
        puts("inserting into map");
        kvs->map_ins(hash, deserial);

        //TODO send our map of addresses back to client
        std::string serial_map = kvs->serializer.serialize_map(kvs->get_servers());

        cout << "Sending: " << serial_map << endl;

        //send client our map of addresses
        server.send_data(&serial_map[0], serial_map.size());
    
    }
}

void KVServer::listen() {
    TCPServerWrapper server(this->port);
    server.start_server();

    void *arg = (void *) this;

    server.recv_data(arg, server_func);
}

//void KVServer::send_membership() {

//}

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
    //map_lock->lock();
    //this->servers.insert(std::pair<size_t, Address>(key, addr));
    //map_lock->unlock();
    
    this->ring->add(addr);
}

//Address KVServer::map_get(size_t key) {
    //map_lock->lock();
    //std::map<size_t, Address>::iterator it = this->servers.find(key);
    //return this->ring->get(key);
    //map_lock->unlock();

    //it->first is key, it->second is value
    //return it->second;
//}

KVServer::~KVServer() {
    //delete this->map_lock;
}

//void test_func(void *arg) {

    //KVServer *test = (KVServer *) arg;

    //cout << test->get_host() << endl;
//}

//int main() {
    //auto d = KVServer((char *) "127.0.0.1", 5656);

    //test_func((void *) &d);
//}
