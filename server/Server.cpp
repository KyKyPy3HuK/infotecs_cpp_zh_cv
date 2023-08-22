


#ifndef _SERVER_CPP_
#define _SERVER_CPP_

#include "Server.hpp"

namespace NET
{
	Server::Server(int port)
		:
		domain_(AF_INET),
		type_(SOCK_STREAM),
        listeningSockfd_(-1),
        clientSockfd_(-1),
		port_(port),
		ipAddress_("0.0.0.0"),
		serverAddress_(),
        clientAddress_()
	{
		serverAddress_.sin_addr.s_addr = htons(INADDR_ANY);
		serverAddress_.sin_family = domain_;
		serverAddress_.sin_port = htons(port_);
	}
	
	Server::Server(int port, std::string ip)
		:
		domain_(AF_INET),
		type_(SOCK_STREAM),
        listeningSockfd_(-1),
        clientSockfd_(-1),
		port_(port),
		ipAddress_(ip),
		serverAddress_(),
        clientAddress_()
	{
		serverAddress_.sin_addr.s_addr = inet_addr(ipAddress_.c_str());
		serverAddress_.sin_family = domain_;
		serverAddress_.sin_port = htons(port_);
	}

	Server::Server(int domain, int type, int port)
		:
		domain_(domain),
		type_(type),
        listeningSockfd_(-1),
        clientSockfd_(-1),
		port_(port),
		ipAddress_("0.0.0.0"),
		serverAddress_(),
        clientAddress_()
	{
		serverAddress_.sin_addr.s_addr = htons(INADDR_ANY);
		serverAddress_.sin_family = domain_;
		serverAddress_.sin_port = htons(port_);
	}

	bool Server::start()
	{
		init();
        while (true)
        {	
			acceptSocket();
			while (true)
			{
				if (!(
					receiveMessage()
					&&
					processMessage()
					&&
					sendMessage(std::string("success\n"))
				))
				{
					break;
				}
				
			}
			close(clientSockfd_);
        }
	}

	bool Server::init()
	{
		if (
			createSocket() 
			&& 
			bindSocket()
			&&
			startListening())
		{
			std::cout << "Server starting at:\t"<< inet_ntoa(serverAddress_.sin_addr) <<":"<< ntohs(serverAddress_.sin_port) << "\n";
			return true;
		}
		std::cerr << "Server error: " << errno <<"\n";
		return false;
	}

	bool Server::createSocket()
	{
		std::cout << "Creating socket: ";
		listeningSockfd_ = socket(domain_, type_, 0);
        if (listeningSockfd_ == -1)
        {
            std::cerr << "failed with error: " << errno << "\n";
            return false;
        }
        std::cout << "success!\n";
		return true;
	}

	bool Server::bindSocket()
	{
		std::cout << "Binding socket: ";
        if(bind(listeningSockfd_, (struct sockaddr*)&serverAddress_, sizeof(serverAddress_)) != 0)
		{
			std::cerr << "failed with error: "<< errno << "\n";
			return false;
		}
        std::cout << "success!\n";
	}

	bool Server::startListening()
	{
		std::cout << "Start listening: ";
		if (listen(listeningSockfd_,1) != 0)
			{
				std::cerr << "listen failed with error: " << errno << "\n";
				return false;
			}
			std::cout << "success!\n";
			return true;
	}

	bool Server::acceptSocket()
	{
		clientSockfd_ = accept(listeningSockfd_,NULL,NULL);
		if (clientSockfd_ == -1)
		{
			std::cerr << "Accepting failed with error: " << errno << "\n";
			close(clientSockfd_);
			return false;
		}
		return true;
	}

	bool Server::closeClientSocket()
	{
		std::cout << "Client disconnected\n";
		close(clientSockfd_);
	}

	bool Server::receiveMessage()
	{
		receiveLength_ = recv(clientSockfd_,(void *)buffer_, BUFFER_SIZE, 0);
            if(receiveLength_ <= 0) {
				return false;
			}
		return true;
	}

	bool Server::processMessage()
	{	
		int recValue = std::stoi(buffer_);
		if (
			(recValue >= 10) 
			&& 
			((recValue % 32) == 0))
		{
			std::cout << "get data from client: " << buffer_ << "\n";
		}
		else{
			std::cout << "Recv Error\n";
		}
		memset(buffer_, 0, BUFFER_SIZE);
		return true;
	}

	bool Server::sendMessage(std::string message)
	{	
		sendLength_ = send(clientSockfd_,message.c_str(),message.size(),0);
		if (sendLength_ <= 0)
		{
			std::cerr << "sending failed\n";
			return false;
		}
		return true;
	}

	Server::~Server()
	{
        close(listeningSockfd_);
        close(clientSockfd_);
	}
}
#endif //_SERVER_CPP_