/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:14:08 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/25 17:56:11 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::signal = false;

Server::Server(){}

Server::~Server(){}

Server::Server(int port, std::string pass) : _port(port), _activeUsers(0), _activeChannels(0), \
 _name("42IRC"), _password(pass), _shutdown(false)
{
	
	for (size_t i = 0; i < _password.size(); i++)
	{
		if (_password[i] == 32 || (_password[i] >= 9 && _password[i] <= 13))
			throw (std::runtime_error("Invalid password."));
	}
	
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
	{
		throw std::runtime_error("Server socket error");
	}
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);

	int reuse = 1;
	
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) ==
		-1)
	{
		close(_socket);
		throw std::runtime_error("Error setting socket options");
	}
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
	{
		close(_socket);
		throw std::runtime_error("Bind error");
	}
	if (listen(_socket, MAX_USERS) == -1)
	{
		close(_socket);
		throw std::runtime_error("Listen error");
	}
	memset(_fds, 0, sizeof(_fds));
	_fds[0].fd = _socket;
	_fds[0].events = POLLIN;

	commandInit();
	std::cout << WHITE_BOLD << _name << " started on " << _port << RESET << std::endl;
}

void	Server::run()
{
	while (this->signal == false)
	{
		_pollRet = poll(_fds, _activeUsers + 1, -1);
		if (_pollRet == -1)
		{
			closeSocket();
			throw (std::runtime_error("Failed poll()."));
		}

		if (_fds[0].revents & POLLIN)
			createUser();
		
		listenUser();
	}
}

void	Server::createUser()
{
	sockaddr_in userAddr;
	socklen_t userSize = sizeof(sockaddr_in);
	int userSocket = accept(_socket, (sockaddr *)&userAddr, &userSize);
	if (userSocket == -1)
	    std::cerr << RED << "Error user connection" << RESET << std::endl;        
	else
	{
		if (_activeUsers < MAX_USERS)
		{
			User user(userSocket, userAddr);
			_activeUsers++;
			user.setId(_activeUsers);
			usersManage[userSocket] = user;
			_fds[_activeUsers].fd = userSocket;
			_fds[_activeUsers].events = POLLIN;

			send(userSocket, "\033[1;32mWelcome to IRC Server!\n\n\033[0m", 36, 0);
			send(userSocket, "\033[1;31mSend PASS <password>\n\033[0m", 33, 0);
			send(userSocket, "\033[1;31mthen send NICK <nickname>\n\033[0m", 38, 0);
			send(userSocket, "\033[1;31mand finally USER <user>\n\033[0m", 36, 0);
			send(userSocket, "\033[1;31mto connect to the server!\n\033[0m", 38, 0);
		} 
		else
		{
			std::cerr << RED << "Max users number reach" << RESET << std::endl;
			close(userSocket);
		}
	}
}

// void Server::listenUser()
// {
//     for (int i = 1; i <= _activeUsers; i++)
//     {
//         if (_fds[i].revents & POLLIN)
//         {
//             char buffer[1024];
//             int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
//             if (bytesRead > 0)
//             {
//                 buffer[bytesRead] = '\0';
//                 std::cout << "Received from user " << usersManage[_fds[i].fd].getNickName() << ": " << buffer << std::endl;
//                 usersManage[_fds[i].fd].setBuf(buffer);
//                 useCommand(usersManage[_fds[i].fd]);
//                 usersManage[_fds[i].fd].getBuf().clear();
//             }
//             else
//             {
//                 deleteFromChannel(usersManage[_fds[i].fd]);
//                 deleteUser(usersManage[_fds[i].fd]);
//                 i--;
//             }
//         }
//     }
// }

void Server::listenUser()
{
    static std::map<int, std::string> inputBuffers;

    for (int i = 1; i <= _activeUsers; i++)
    {
        if (_fds[i].revents & POLLIN)
        {
            char buffer[1024];
            int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                inputBuffers[_fds[i].fd] += buffer;

                // Vérifier si la commande est complète (présence d'un '\n')
                size_t newlinePos = inputBuffers[_fds[i].fd].find('\n');
                if (newlinePos != std::string::npos)
                {
                    std::string completeCommand = inputBuffers[_fds[i].fd].substr(0, newlinePos);
                    inputBuffers[_fds[i].fd] = inputBuffers[_fds[i].fd].substr(newlinePos + 1);

                    std::cout << "Received from user " << usersManage[_fds[i].fd].getNickName() << ": " << completeCommand << std::endl;
                    usersManage[_fds[i].fd].setBuf(completeCommand);
                    useCommand(usersManage[_fds[i].fd]);
                    usersManage[_fds[i].fd].getBuf().clear();
                }
            }
            else if (bytesRead == 0)
            {
                // La connexion a été fermée, traiter les données restantes s'il y en a
                if (!inputBuffers[_fds[i].fd].empty())
                {
                    std::cout << "Received final data from user " << usersManage[_fds[i].fd].getNickName() << ": " << inputBuffers[_fds[i].fd] << std::endl;
                    usersManage[_fds[i].fd].setBuf(inputBuffers[_fds[i].fd]);
                    useCommand(usersManage[_fds[i].fd]);
                    usersManage[_fds[i].fd].getBuf().clear();
                }
                deleteFromChannel(usersManage[_fds[i].fd]);
                deleteUser(usersManage[_fds[i].fd]);
                inputBuffers.erase(_fds[i].fd);
                i--;
            }
            else
            {
                std::cerr << "Error receiving data from user " << usersManage[_fds[i].fd].getNickName() << std::endl;
            }
        }
    }
}

// void	Server::deleteUser(User &user)
// {
// 	std::map<int, User>::iterator it = usersManage.find(user.getSocket());
// 	if (it != usersManage.end())
// 	{		
// 		close(user.getSocket());
// 		std::cout << RED << "User " << user.getId() << " disconnected" << RESET << std::endl;
// 		usersManage.erase(it);
// 		_activeUsers--;
// 	}
// 	for (int j = user.getId(); j <= _activeUsers; ++j)
// 		_fds[j] = _fds[j + 1];	
// }

void Server::deleteUser(User &user)
{
    std::map<int, User>::iterator it = usersManage.find(user.getSocket());
    if (it != usersManage.end())
    {
        int userSocket = user.getSocket();
        close(userSocket);
        for (int j = user.getId(); j <= _activeUsers; ++j)
            _fds[j] = _fds[j + 1];

        _activeUsers--;
    	std::cout << RED << "User " << user.getId() << " disconnected" << RESET << std::endl;
        usersManage.erase(it);
    }
}

