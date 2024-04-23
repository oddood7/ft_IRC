/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diavolo <diavolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:56:58 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/23 14:47:29 by diavolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
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
}

Server::~Server()
{
	close(_socket);
}

void Server::run()
{
	sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_size);
	if (client_socket == -1)
	{
		std::cerr << "Failed to accept client connection" << std::endl;
		return;
	}
	std::cout << "New client connected" << std::endl;

	close(client_socket);
}
