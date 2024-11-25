/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:14:08 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 15:42:43 by lde-mais         ###   ########.fr       */
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

void Server::listenUser()
{
    static std::map<int, std::string> partialCommands;

    for (int i = 1; i <= _activeUsers; i++)
    {
        if (_fds[i].revents & POLLIN)
        {
            char buffer[1024];
            int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
				std::cout << "Auto command received from user " << i << ": " << buffer;
                partialCommands[_fds[i].fd] += buffer;

                size_t pos;
                while ((pos = partialCommands[_fds[i].fd].find('\n')) != std::string::npos)
                {
                    std::string command = partialCommands[_fds[i].fd].substr(0, pos);
                    partialCommands[_fds[i].fd] = partialCommands[_fds[i].fd].substr(pos + 1);

					std::cout << "Processing command: " << command << std::endl;
					
                    usersManage[_fds[i].fd].setBuf(command);
                    useCommand(usersManage[_fds[i].fd]);
                    usersManage[_fds[i].fd].getBuf().clear();
                }
            }
            else if (bytesRead == 0)
            {
                if (!partialCommands[_fds[i].fd].empty())
                {
                    usersManage[_fds[i].fd].setBuf(partialCommands[_fds[i].fd]);
                    useCommand(usersManage[_fds[i].fd]);
                    usersManage[_fds[i].fd].getBuf().clear();
                }
                deleteFromChannel(usersManage[_fds[i].fd]);
                deleteUser(usersManage[_fds[i].fd]);
                partialCommands.erase(_fds[i].fd);
                i--;
            }
            else
            {
                deleteFromChannel(usersManage[_fds[i].fd]);
                deleteUser(usersManage[_fds[i].fd]);
                partialCommands.erase(_fds[i].fd);
                i--;
            }
        }
    }
}

// void Server::deleteUser(User &user)
// {
//     std::map<int, User>::iterator it = usersManage.find(user.getSocket());
//     if (it != usersManage.end())
//     {
//         int userSocket = user.getSocket();
//         close(userSocket);
//         for (int j = user.getId(); j <= _activeUsers; ++j)
//             _fds[j] = _fds[j + 1];

//         _activeUsers--;
//     	std::cout << RED << "User " << user.getId() << " disconnected" << RESET << std::endl;
//         usersManage.erase(it);
//     }
// }

// void Server::deleteUser(User &user)
// {
//     std::string nickname = user.getNickName();
    
//     // Nettoyage de tous les canaux d'abord
//     for (size_t i = 0; i < _channelsList.size();) {
//         _channelsList[i].removeChatter(nickname);
//         _channelsList[i].removeOperator(nickname);
//         _channelsList[i].removeInvite(nickname);
        
//         // Si le canal est vide après le retrait de l'utilisateur
//         if (_channelsList[i].getChatters().empty()) {
//             // Si c'était le dernier canal
//             if (_activeChannels > 0)
//                 _activeChannels--;
            
//             // Supprimer le canal
//             _channelsList.erase(_channelsList.begin() + i);
//         }
//         else {
//             // Si le propriétaire du canal part, transférer la propriété au premier opérateur restant
//             if (_channelsList[i].getOwner() == nickname && !_channelsList[i].getOperator().empty()) {
//                 _channelsList[i].setOwner(_channelsList[i].getOperator()[0]);
//             }
//             ++i;
//         }
//     }

//     // Supprimer l'utilisateur
//     std::map<int, User>::iterator it = usersManage.find(user.getSocket());
//     if (it != usersManage.end()) {
//         int userSocket = user.getSocket();
//         close(userSocket);
        
//         // Mettre à jour les descripteurs de fichiers
//         for (int j = user.getId(); j <= _activeUsers; ++j) {
//             _fds[j] = _fds[j + 1];
//         }

//         _activeUsers--;
//         std::cout << RED << "User " << user.getId() << " disconnected" << RESET << std::endl;
//         usersManage.erase(it);
//     }
// }

void Server::deleteUser(User &user)
{
    int userSocket = user.getSocket();
    int userId = user.getId();
    std::string userNick = user.getNickName();
    std::string userName = user.getUserName();
    std::string userHost = user.getHost();
    
    std::map<int, User>::iterator it = usersManage.find(userSocket);
    if (it == usersManage.end()) {
        return;
    }

    for (size_t i = 0; i < _channelsList.size(); ++i) {
        std::vector<std::string>& chatters = _channelsList[i].getChatters();
        
        for (size_t j = 0; j < chatters.size(); ++j) {
            if (chatters[j] == userNick) {
                std::string quitMsg = ":" + userNick + "!" + userName + "@" + userHost + 
                                    " QUIT :Connection closed\r\n";
                
                for (size_t k = 0; k < chatters.size(); ++k) {
                    if (chatters[k] != userNick) {
                        User* otherUser = getUserByNickname(chatters[k]);
                        if (otherUser && otherUser->getSocket() != userSocket) {
                            send(otherUser->getSocket(), quitMsg.c_str(), quitMsg.size(), 0);
                        }
                    }
                }
                
                chatters.erase(chatters.begin() + j);
                break;
            }
        }
        
        if (_channelsList[i].getChatters().empty()) {
            _channelsList.erase(_channelsList.begin() + i);
            if (_activeChannels > 0)
                _activeChannels--;
            i--;
        }
    }

    close(userSocket);

    for (int j = userId; j <= _activeUsers; ++j) {
        _fds[j] = _fds[j + 1];
    }

    if (_activeUsers > 0)
        _activeUsers--;
    
    std::cout << RED << "User disconnected" << RESET << std::endl;
    usersManage.erase(it);
}