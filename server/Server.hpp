


#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

namespace NET
{
    class Server
    {
        // fields
    private:
        int domain_;
        int type_;

        int listeningSockfd_;
        int clientSockfd_;

        std::string ipAddress_;
        int port_;
        struct sockaddr_in serverAddress_;
        struct sockaddr_in clientAddress_;

        char buffer_[BUFFER_SIZE];
        std::string message_;
        int sendLength_;
        int receiveLength_;

        // constructors
    public:
        Server(int port);
        Server(int port, std::string ip);
        Server(int domain, int type, int port);
        ~Server();

        // functions
    public:
        bool start();
        bool stop();
    private:
        bool init();
        bool createSocket();
        bool bindSocket();
        bool startListening();
        bool acceptSocket();
        bool closeClientSocket();
        bool receiveMessage();
        bool processMessage();
        bool sendMessage(std::string message);
    };
}

#endif // !_SERVER_HPP_
