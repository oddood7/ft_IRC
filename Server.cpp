/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:14:08 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/21 12:49:16 by lde-mais         ###   ########.fr       */
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

// void Server::listenUser()
// {
//     // Cette map stocke les commandes partielles de chaque utilisateur.
//     static std::map<int, std::string> partialCommands;

//     for (int i = 1; i <= _activeUsers; i++)
//     {
//         if (_fds[i].revents & POLLIN)
//         {
//             char buffer[1024];
//             int bytesRead = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
//             if (bytesRead > 0)
//             {
//                 buffer[bytesRead] = '\0';  // Terminer le buffer par '\0'
//                 partialCommands[_fds[i].fd] += buffer;  // Ajouter la nouvelle partie reçue au buffer existant

//                 // Processer les commandes complètes trouvées dans le buffer
//                 std::string::size_type pos;
//                 while ((pos = partialCommands[_fds[i].fd].find('\n')) != std::string::npos)
//                 {
//                     // Extraire la commande jusqu'au '\n'
//                     std::string command = partialCommands[_fds[i].fd].substr(0, pos);
//                     partialCommands[_fds[i].fd] = partialCommands[_fds[i].fd].substr(pos + 1);

//                     // Nettoyage manuel des retours chariot `\r`
//                     for (std::string::iterator it = command.begin(); it != command.end(); ++it)
//                     {
//                         if (*it == '\r')
//                         {
//                             command.erase(it);  // Supprimer les '\r'
//                             break;  // Ne faire qu'une seule fois par commande
//                         }
//                     }

//                     // Exécuter la commande complète
//                     usersManage[_fds[i].fd].setBuf(command);
//                     useCommand(usersManage[_fds[i].fd]);
//                     usersManage[_fds[i].fd].getBuf().clear();
//                 }
//             }
//             else if (bytesRead == 0)  // Gestion de la déconnexion avec ou sans données restantes
//             {
//                 if (!partialCommands[_fds[i].fd].empty())
//                 {
//                     // Si des données restent dans le buffer, les traiter comme une commande finale
//                     usersManage[_fds[i].fd].setBuf(partialCommands[_fds[i].fd]);
//                     useCommand(usersManage[_fds[i].fd]);
//                     usersManage[_fds[i].fd].getBuf().clear();
//                 }
//                 // Supprimer l'utilisateur et nettoyer le buffer partiel
//                 deleteFromChannel(usersManage[_fds[i].fd]);
//                 deleteUser(usersManage[_fds[i].fd]);
//                 partialCommands.erase(_fds[i].fd);
//                 i--;  // Réduire l'indice pour éviter de sauter un utilisateur
//             }
//             else  // Gestion des erreurs
//             {
//                 deleteFromChannel(usersManage[_fds[i].fd]);
//                 deleteUser(usersManage[_fds[i].fd]);
//                 partialCommands.erase(_fds[i].fd);
//                 i--;  // Réduire l'indice ici aussi
//             }
//         }
//     }
// }


// void Server::processCommands(User &user, std::string &buffer)
// {
//     std::string::size_type pos;
//     while ((pos = buffer.find('\n')) != std::string::npos)
//     {
//         std::string line = buffer.substr(0, pos);
//         buffer.erase(0, pos + 1);

//         // Supprimer les retours chariot
//         line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

//         // Passer la ligne complète à setBuf
//         user.setBuf(line);

//         if (!user.getBuf().empty())
//         {
//             std::string command = ft_toupper(user.getBuf()[0]);
            
//             if (command == "CAP" || command == "PASS" || command == "NICK" || command == "USER")
//             {
//                 useCommand(user);
//             }
//             else if (user.getVerif())
//             {
//                 useCommand(user);
//             }
//             else
//             {
//                 std::string err = ERR_NOTREGISTERED(_name);
//                 send(user.getSocket(), err.c_str(), err.size(), 0);
//             }
//         }
//     }
// }


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
                partialCommands[_fds[i].fd] += buffer;

                size_t pos;
                while ((pos = partialCommands[_fds[i].fd].find('\n')) != std::string::npos)
                {
                    std::string command = partialCommands[_fds[i].fd].substr(0, pos);
                    partialCommands[_fds[i].fd] = partialCommands[_fds[i].fd].substr(pos + 1);

                    usersManage[_fds[i].fd].setBuf(command);
                    useCommand(usersManage[_fds[i].fd]);
                    usersManage[_fds[i].fd].getBuf().clear();
                }
            }
            else if (bytesRead == 0)
            {
                // Gérer CTRL-D ou déconnexion
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

