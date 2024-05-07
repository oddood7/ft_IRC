/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:56:58 by lde-mais          #+#    #+#             */
/*   Updated: 2024/05/07 15:08:53 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
Server::Server() {}

Server::Server(int port) : _port(port)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw std::runtime_error("Server socket error");
	}

	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
	{
		close(_socket);
		throw std::runtime_error("Bind error");
	}

	if (listen(_socket, 5) == -1)
	{
		close(_socket);
		throw std::runtime_error("Listen error");
	}
	_fd[0].fd = _socket;
    _fd[0].events = POLLIN;
}

Server::~Server()
{
	close(_socket);
}

void Server::run()
{
	while (!_stop)
    {
        _pollRet = poll(_fd, _activeClients + 1, -1);
        if (_pollRet == -1)
        {
            throw std::runtime_error("Failed poll().");
        }

        if (_fd[0].revents & POLLIN)
        {
            acceptNewConnection();
        }

        for (int i = 1; i <= _activeClients; ++i)
        {
            if (_fd[i].revents & POLLIN)
            {
                processClientData(i);
            }
        }
    }
}

void Server::acceptNewConnection()
{
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1)
    {
        std::cerr << "Failed to accept client connection" << std::endl;
    }
    else
    {
        std::cout << "New client connected" << std::endl;
        _activeClients++;
        _fd[_activeClients].fd = client_socket;
        _fd[_activeClients].events = POLLIN;
    }
}

void Server::processClientData(int _clientIndex)
{
    char buffer[1024];
    int num_bytes = recv(_fd[_clientIndex].fd, buffer, sizeof(buffer), 0);
    if (num_bytes <= 0)
    {
        // Le client s'est déconnecté
        close(_fd[_clientIndex].fd);
        std::cout << "Client disconnected" << std::endl;
        // Décaler les descripteurs de fichiers pour maintenir la continuité
        for (int i = _clientIndex; i < _activeClients; ++i)
        {
            _fd[i] = _fd[i + 1];
        }
        _activeClients--;
    }
    else
    {
        buffer[num_bytes] = '\0';
        std::cout << "Received data from client " << _clientIndex << ": " << buffer << std::endl;
    }
}