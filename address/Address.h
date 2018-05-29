#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdio.h>

class Address {

    public:
        Address(std::string h, int p) : host(h), port(p) {
        
        }

        operator std::string () const {
            return this->host + ":" + std::to_string(this->port);
        }

        std::string host;
        int port;
};


#endif
