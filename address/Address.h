#ifndef ADDRESS_H
#define ADDRESS_H

class Address {

    public:
        Address(std::string h, int p) : host(h), port(p) {
        
        }

        std::string host;
        int port;
};


#endif
