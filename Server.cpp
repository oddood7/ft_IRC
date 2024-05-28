/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:30:15 by lde-mais          #+#    #+#             */
/*   Updated: 2024/05/28 16:30:17 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string& password) : _port(port), _activeUsers(0), _password(password), _shutdown(false)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw std::runtime_error("Server socket error");
    }

    _address.sin_family = AF_INET;
    _address.sin_port = htons(_port);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        close(_socket);
        throw std::runtime_error("Error setting socket options");
    }

    if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1) {
        close(_socket);
        throw std::runtime_error("Bind error");
    }

    if (listen(_socket, MAX_USERS) == -1) {
        close(_socket);
        throw std::runtime_error("Listen error");
    }

    memset(_fds, 0, sizeof(_fds));
    _fds[0].fd = _socket;
    _fds[0].events = POLLIN;
}

Server::~Server() {
    close(_socket);
}

void Server::run()
{
    while (!_shutdown) {
        int pollRet = poll(_fds, _activeUsers + 1, -1);
        if (pollRet == -1) {
            throw std::runtime_error("Failed poll().");
        }

        if (_fds[0].revents & POLLIN) {
            acceptNewConnection();
        }

        for (int i = 1; i <= _activeUsers; ++i) {
            if (_fds[i].revents & POLLIN) {
                processUserData(i);
            }
        }
    }
}

void Server::acceptNewConnection() {
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_socket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
    }
	else
	{
        if (_activeUsers < MAX_USERS)
		{
            User user(client_socket, client_addr);
            _activeUsers++;
            user.setId(_activeUsers);
            UsersManage[client_socket] = user;
            _fds[_activeUsers].fd = client_socket;
            _fds[_activeUsers].events = POLLIN;
            send(client_socket, "Please authenticate using PASS <password>\n", 43, 0);
            std::cout << "New client connected" << std::endl;
        }
		else
		{
            std::cerr << "Max users number reached" << std::endl;
            close(client_socket);
        }
    }
}

void Server::processUserData(int userIndex)
{
    char buffer[1024];
    int num_bytes = recv(_fds[userIndex].fd, buffer, sizeof(buffer), 0);
    if (num_bytes <= 0)
	{
        // Le user s'est déconnecté
        close(_fds[userIndex].fd);
        std::cout << "User disconnected" << std::endl;
        // Décaler les descripteurs de fichiers pour maintenir la continuité
        for (int i = userIndex; i < _activeUsers; ++i) {
            _fds[i] = _fds[i + 1];
        }
        _activeUsers--;
    }
	else
	{
        buffer[num_bytes] = '\0';
        std::cout << "Received data from user " << userIndex << ": " << buffer << std::endl;
        std::string receivedData(buffer);
        if (receivedData.substr(0, 5) == "PASS ") {
            std::string pass = receivedData.substr(5);
            pass = pass.substr(0, pass.size() - 1);

			//std::cout << "Received password: '" << pass << "'" << std::endl;

            if (authenticateUser(pass))
			{
                std::cout << "User authenticated successfully" << std::endl;
                UsersManage[_fds[userIndex].fd].setAuthenticated(true);
                send(_fds[userIndex].fd, "Welcome to the IRC server!\n", 26, 0);
				send(_fds[userIndex].fd, "\n", 1, 0);
            }
			else
			{
                std::cerr << "User failed to authenticate\n" << std::endl;
                close(_fds[userIndex].fd);
                for (int i = userIndex; i < _activeUsers; ++i) {
                    _fds[i] = _fds[i + 1];
                }
                _activeUsers--;
                UsersManage.erase(_fds[userIndex].fd);
            }
        }
    }
}

