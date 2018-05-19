#ifndef KVRING_H 
#define KVRING_H 
#include <map>
#include <vector>

template <typename V>
class KVRing {

    public:
        KVRing(int replicas, std::vector<V> nodes) : num_replicas(replicas) {

            for(V &node : nodes) {
                //taking advantage of overriden operator in Address class
                std::string str = node;
                for(int i = 0; i < replicas; i++) {
                    size_t hash = this->hash_func(str + std::to_string(i));
                    this->ring.insert(std::pair<size_t, V>(hash, node));
                }
            }
        }

        V get(std::string key) {
            size_t hash = this->hash_func(key);

            typename std::map<size_t, V>::iterator itup = this->ring.upper_bound(hash);

            //this means we reached end, this range belongs to first key in the map
            if(itup == this->ring.end()) {
                return ring.first();
            }

            else {
            
            }

            //for (; itup != this->ring.end(); ++itup) {
                ////std::cout << itup->first << " => " << itup->second << '\n';
            //}
        }


    private:
        int num_replicas;
        std::map<size_t, V> ring;
        std::hash<std::string> hash_func;


};

//#include "KVring.cc"

#endif
