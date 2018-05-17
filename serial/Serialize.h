#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <string>
#include <map>

template <typename K, typename V>
class Serialize {

    public:
        virtual std::string serialize_addr(std::string host, int port) = 0;
        virtual std::string serialize_map(std::map<K, V> map) = 0;
        virtual V deserialize_addr(std::string str) = 0;
};




#endif
