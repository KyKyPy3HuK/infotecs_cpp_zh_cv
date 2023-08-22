


#ifndef _SERVER_CPP_
#define _SERVER_CPP_

#include "Client.hpp"

namespace NET
{

	Client::Client(int port, std::string ip)
		:
		domain_(AF_INET),
		type_(SOCK_STREAM),
        clientSockfd_(-1),
		port_(port),
		ipAddress_(ip),
        clientAddress_()
	{
		clientAddress_.sin_addr.s_addr = inet_addr(ipAddress_.c_str());
		clientAddress_.sin_family = domain_;
		clientAddress_.sin_port = htons(port_);
	}

	bool Client::tryConnection()
	{
		return init();
	}

	bool Client::closeConnection(){
		close(clientSockfd_);
		return true;
	}

	bool Client::init()
	{
		if  (
		    createSocket() 
			&& 
			connectToServer()
            )
		{
			return true;
		}
		std::cerr << "Client error: " << errno <<"\n";
		return false;
	}

	bool Client::createSocket()
	{
		clientSockfd_ = socket(domain_, type_, 0);
        if (clientSockfd_ == -1)
        {
            std::cerr << "Creating socket failed with error: " << errno << "\n";
            return false;
        }
		return true;
	}

	bool Client::connectToServer()
	{
        if(connect(clientSockfd_, (struct sockaddr*)&clientAddress_, sizeof(clientAddress_)) == -1)
		{
			std::cerr << "Connecting to server failed with error: "<< errno << "\n";
			return false;
		}
		return true;
	}

	bool Client::receiveMessage()
	{
		receiveLength_ = recv(clientSockfd_, buffer_, BUFFER_SIZE, 0);
            if(receiveLength_ < 0) {
				std::cout << "Receive failed with error: " << errno << "\n";
				return false;
			}
			else if (receiveLength_ == 0)
			{
				std::cout << "Server disconnected\n";
			}
		return true;
	}

	bool Client::processMessage()
	{
		std::cout << "Server: "<< buffer_ << "\n";
		return true;
	}

	int Client::sendMessage(std::string message)
	{	
		sendLength_ = send(clientSockfd_,message.c_str(),message.size(),0);
		if (sendLength_ < 0)
		{
			std::cerr << "Sending failed with error: " << errno << "\n";
			return -1;
		}
		else if (sendLength_ == 0)
		{
			std::cout << "Server disconnected\n";
			return 0;
		}
		
		return sendLength_;
	}

	Client::~Client()
	{
        close(clientSockfd_);
	}
}
#endif //_SERVER_CPP_