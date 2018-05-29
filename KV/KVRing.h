#ifndef KVRING_H 
#define KVRING_H 
#include <map>
#include <vector>
#include <mutex>

template <typename V>
class KVRing {

    public:
        KVRing(int replicas, std::vector<V> nodes = {}) : num_replicas(replicas) {
            this->init();

            for(V &node : nodes) {
                //taking advantage of overriden operator in Address class
                this->add(node);
            }
        }

        const std::map<size_t, V>& get_map() {
            //TODO make this unmodifiable;
            return this->ring;
        }


        V get(std::string key) {
            this->map_lock.lock();
            size_t hash = this->hash_func(key);

            typename std::map<size_t, V>::iterator itup = this->ring.upper_bound(hash);

            //this means we reached end, this range belongs to first key in the map
            if(itup == this->ring.end()) {
                this->map_lock.unlock();
                return ring.first();
            }

            else {
                //itup points to the next element, which is the node that should handle
                //this particular key
                this->map_lock.unlock();
                return itup->second;
            }
        }

        void remove(V node) {
            this->map_lock.lock();
            std::string str = node;
            for(int i = 0; i < this->num_replicas; i++) {
                size_t hash = this->hash_func(str + std::to_string(i));
                this->ring.erase(hash);
            }
            this->map_lock.unlock();
        }

        void add(V node) {
            std::string str = node;
            this->map_lock.lock();
            for(int i = 0; i < this->num_replicas; i++) {
                size_t hash = this->hash_func(str + std::to_string(i));
                this->ring.insert(std::pair<size_t, V>(hash, node));
            }
            this->map_lock.unlock();
        }

    private:
        int num_replicas;
        std::map<size_t, V> ring;
        std::hash<std::string> hash_func;
        std::mutex map_lock;

        void init() {
            //this->map_lock = new std::mutex();
        }
};

//#include "KVring.cc"

#endif
