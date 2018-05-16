#include "KVPart.h"


void KVPart::send_to_seeds() {

    for(auto const &seed : this->seeds) {
        //TODO tell seed that server has joined the ring, using TCP packet
    }
}

//int main() {
    //std::vector<KVSeed> seeds;
    //auto d = new KVPart((char *) "127.0.0.1", 5000, (char *) "127.0.0.1", 6379, seeds);
    //d->send_to_seeds();
//}
