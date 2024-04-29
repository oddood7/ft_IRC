/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diavolo <diavolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:56:58 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/29 13:15:31 by diavolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
Server::Server() {}

Server::Server(int port) : _port(port)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw std::invalid_argument("socket creation failed");
}

Server::~Server()
{
	close(_serverSocket);
}

void Server::run()
{
	memset(&_serverAddr, 0, sizeof(_serverAddr));
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_client_number = 0;
	
	if (bind(_serverSocket, (sockaddr*)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw std::logic_error("bind error");
	if (listen(_serverSocket, 10) == -1)
		throw std::logic_error("listen error");
	
	std::cout << "waiting for connexion on the port " << _port << std::endl;
	
	fds[0].fd = _serverSocket;
	fds[0].events = POLLIN;

	while (true){
		int ret = poll(fds, 1, -1);
		if (ret == -1)
			throw std::logic_error("Poll failed");
		Client test(_serverSocket);
		vec_client.insert(test.getIPAddress());
		map_client[vec_client] = test;
		if (ret > 0)
		{
			if (fds[0].revents & POLLIN){
				
				
				std::cout << "Connexion accepted since " << inet_ntoa(test.getCLientAddr().sin_addr)
				<< ":" << ntohs(test.getCLientAddr().sin_port) << std::endl;

				while (true){
					_bytesRead = recv(map_client[test.getCLientAddr().sin_addr.s_addr].getCLientSocket(), &_buff, 1024, MSG_DONTWAIT);
					if (_bytesRead <= 0){
						if (_bytesRead == 0)
						{
							std::cout << "Client disconnected" << std::endl;
							break;
						}
						else
							throw std::runtime_error("Error recv");	
					}
					_buff[_bytesRead] = '\0';
					std::cout << "Message from the client: " << _buff << std::endl;
				}
			}
		}
	}
}
