


#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

namespace NET
{
    class Client
    {
        // fields
    private:
        int domain_;
        int type_;

        int clientSockfd_;

        std::string ipAddress_;
        int port_;
        struct sockaddr_in clientAddress_;

        char buffer_[BUFFER_SIZE];
        int sendLength_;
        int receiveLength_;

        // constructors
    public:
        Client(int port);
        Client(int port, std::string ip);
        Client(int domain, int type, int port);
        ~Client();

        // functions
    public:
        bool tryConnection();
        bool closeConnection();
        int sendMessage(std::string message);

    private:
        bool init();
        bool createSocket();
        bool connectToServer();
        bool closeSocket();
        bool receiveMessage();
        bool processMessage();
    };
}

#endif // !_SERVER_HPP_
