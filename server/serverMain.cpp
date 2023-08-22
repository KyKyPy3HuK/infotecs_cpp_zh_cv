


#ifndef _SERVER_MAIN_CPP_
#define _SERVER_MAIN_CPP_

#include "serverMain.hpp"
#include "Server.hpp"

int main(){
    NET::Server server(DEFAULT_PORT);

    server.start();
    return 0;
}

#endif //_SERVER_MAIN_CPP_