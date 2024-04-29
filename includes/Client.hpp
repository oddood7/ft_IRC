/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diavolo <diavolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:20:29 by diavolo           #+#    #+#             */
/*   Updated: 2024/04/29 13:16:16 by diavolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client{
    private:
    int _clientSocket;
    sockaddr_in _clientAddr;
	socklen_t _clientAddrLen;
    const std::string ipClient;
    Client(){};
    
    public:
    int getCLientSocket(){
        return this->_clientSocket;
    };
    sockaddr_in getCLientAddr() const {
        return this->_clientAddr;
    };
    socklen_t getClientAddrLen()const {
        return this->_clientAddrLen;
    };
    std::string getIPAddress() const {
        return this->ipClient;
    }
    
    Client(int serverSocket){
        _clientAddrLen = sizeof(_clientAddr);
        _clientSocket = accept(serverSocket, (sockaddr *)&_clientAddr, &_clientAddrLen);
				if (_clientSocket == -1)
					throw std::logic_error("Accept connexion error");
        char ipBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(_clientAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
        ipClient = ipBuffer;
    };
    ~Client(){
        close(_clientSocket);
    };
};


#endif