#include "Serialize.h"
#include "../address/Address.h"
#include <vector>
#include <sstream>

class KVSerialize : public Serialize<size_t, Address> {

    public:
        //std::string serialize_read_req(std::string key);
        //std::string serialize_write_req(std::string key); 
        std::string serialize_addr(std::string host, int port);
        std::string serialize_map(std::map<size_t, Address> map);
        Address deserialize_addr(std::string str);

        std::vector<Address> deserialize_addr_arr(std::string str);

};
