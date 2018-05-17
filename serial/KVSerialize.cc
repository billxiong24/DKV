#include "KVSerialize.h"

#define DELIM ':'
#define ARR_DELIM '$'

static std::vector<std::string> explode_str(std::string input, char delim) {

    std::istringstream stream(input);
    std::string token;

    std::vector<std::string> arr;

    while(std::getline(stream, token, delim)) {
        arr.push_back(token);
    }

    return arr;
}

std::string KVSerialize::serialize_addr(std::string host, int port) {
    std::string str = std::string(host) + ":" + std::to_string(port);
    return str;
}

std::string KVSerialize::serialize_map(std::map<size_t, Address> map) {
    std::string ret;

    for(auto &pair : map) {
        Address addr = pair.second;
        std::string addr_str = serialize_addr(addr.host, addr.port);
        ret += addr_str;
        ret += ARR_DELIM;
    }

    //string will have an extra delimiter, remove it
    ret.pop_back();
    ret += '\0';

    return ret;
}

Address KVSerialize::deserialize_addr(std::string str) {
    std::vector<std::string> info = explode_str(str, DELIM);
    return Address(info[0], std::stoi(info[1]));
}


std::vector<Address> KVSerialize::deserialize_addr_arr(std::string res) {
    std::vector<std::string> addresses = explode_str(res, ARR_DELIM);

    std::vector<Address> result;

    for(auto &str_addr: addresses) {
        Address addr = this->deserialize_addr(str_addr);
        result.push_back(addr);
    }

    return result;
}
