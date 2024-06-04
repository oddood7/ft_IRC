/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:30:15 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/04 15:28:15 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string &password) : _port(port),
	_activeUsers(0), _password(password), _shutdown(false)
{
	int	reuse;

	_commandFunctions["/CREATE"] = &Server::createChannel;
	_commandFunctions["/JOIN"] = &Server::joinChannel;
	_commandFunctions["/PART"] = &Server::partChannel;
	_commandFunctions["PASS"] = &Server::login;
	// _commandFunctions["/NICK"] = &Server::setNick;
	// _commandFunctions["/QUIT"] = &Server::quit;
	// _commandFunctions["/PRIVMSG"] = &Server::sendMsg;
	// _commandFunctions["/USER"] = &Server::setUser;
	// _commandFunctions["/OPER"] = &Server::becomeOper;
	// _commandFunctions["/KICK"] = &Server::kickUser;
	// _commandFunctions["/INVITE"] & Server::inviteUser;
	// _commandFunctions["/TOPIC"] = &Server::setTopic;
	// _commandFunctions["/MODE"] = &Server::setMode;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
	{
		throw std::runtime_error("Server socket error");
	}
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	reuse = 1;
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
}

Server::~Server()
{
	close(_socket);
}

void Server::run()
{
	int	pollRet;

	while (!_shutdown)
	{
		pollRet = poll(_fds, _activeUsers + 1, -1);
		if (pollRet == -1)
		{
			throw std::runtime_error("Failed poll().");
		}
		if (_fds[0].revents & POLLIN)
		{
			acceptNewConnection();
		}
		for (int i = 1; i <= _activeUsers; ++i)
		{
			if (_fds[i].revents & POLLIN)
			{
				processUserData(i);
			}
		}
	}
}

int Server::getUserIndex(int fd)
{
	for (int i = 0; i < _activeUsers; ++i)
	{
		if (_fds[i].fd == fd)
		{
			return (i);
		}
	}
	return (-1); // Retourne -1 si le descripteur de fichier n'est pas trouvé
}

void Server::acceptNewConnection()
{
	sockaddr_in	client_addr;
	socklen_t	client_addr_size;
	int			client_socket;

	client_addr_size = sizeof(client_addr);
	client_socket = accept(_socket, (struct sockaddr *)&client_addr,
		&client_addr_size);
	if (client_socket == -1)
	{
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
			send(client_socket, "Please authenticate using PASS <password>\n",
				43, 0);
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
	char	buffer[1024];
	int		num_bytes;

	num_bytes = recv(_fds[userIndex].fd, buffer, sizeof(buffer), 0);
	if (num_bytes <= 0)
	{
		// Le user s'est déconnecté
		close(_fds[userIndex].fd);
		std::cout << "User disconnected" << std::endl;
		// Décaler les descripteurs de fichiers pour maintenir la continuité
		for (int i = userIndex; i < _activeUsers; ++i)
		{
			_fds[i] = _fds[i + 1];
		}
		_activeUsers--;
	}
	else
	{
		buffer[num_bytes] = '\0';
		std::cout << "Received data from user " << userIndex << ": " << buffer << std::endl;
		std::string receivedData(buffer);
		// Tokenisez la donnée reçue pour séparer la commande et les arguments
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream iss(receivedData);
		while (std::getline(iss, token, ' '))
		{
			std::istringstream subIss(token);
			std::string subToken;
			while (std::getline(subIss, subToken, '\n'))
			{
				if (!subToken.empty())
				{
					tokens.push_back(subToken);
				}
			}
		}
		if (tokens.empty())
		{
			// La donnée reçue est invalide, gérez l'erreur
			return ;
		}
		std::string command = tokens[0];
		std::map<std::string,
			CommandFunction>::iterator it = _commandFunctions.find(command);
		if (it != _commandFunctions.end())
		{
			User &user = UsersManage[_fds[userIndex].fd];
			std::string args = tokens.size() > 1 ? tokens[1] : "";
			(this->*(it->second))(user, args);
		}
		else
		{
			std::cout << "Unknown command" << std::endl;
			// Commande inconnue, gérez l'erreur
		}
	}
}

void Server::login(User &user, const std::string &password)
{
	int	userIndex;

	// Vérifier si le mot de passe est correct
	if (password == _password)
	{
		// Authentifier l'utilisateur
		user.setAuthenticated(true);
		// Envoyer un message de bienvenue à l'utilisateur
		std::string welcomeMessage = RPL_WELCOME(std::to_string(user.getId()), user.getNick());
		send(user.getSocket(), welcomeMessage.c_str(), welcomeMessage.length(),
			0);
		std::cout << "User authenticated successfully" << std::endl;
	}
	else
	{
		// Le mot de passe est incorrect, déconnecter l'utilisateur
		std::cerr << "User failed to authenticate" << std::endl;
		close(user.getSocket());
		// Retirer l'utilisateur de la liste des utilisateurs connectés
		std::map<int, User>::iterator it = UsersManage.find(user.getSocket());
		if (it != UsersManage.end())
		{
			UsersManage.erase(it);
		}
		// Décaler les descripteurs de fichiers pour maintenir la continuité
		userIndex = getUserIndex(user.getSocket());
		for (int i = userIndex; i < _activeUsers; ++i)
		{
			_fds[i] = _fds[i + 1];
		}
		_activeUsers--;
	}
}
void Server::createChannel(User &user, const std::string &channelName)
{
	(void)user;
	(void)channelName;
	std::cout << "into Create" << std::endl;
	// Implémentation de la création d'un canal
}

void Server::joinChannel(User &user, const std::string &channelName)
{
	(void)user;
	(void)channelName;
	std::cout << "into join" << std::endl;
	// Implémentation du fait de rejoindre un canal
}

void Server::partChannel(User &user, const std::string &channelName)
{
	(void)user;
	(void)channelName;
	std::cout << "into part" << std::endl;
	// Implémentation du fait de quitter un canal
}

void Server::setNick(User &user, const std::string &nick)
{
	user.setNick(nick);
}

void Server::setUser(User &user, const std::string &userInfo)
{
	(void)userInfo;
	user.setAuthenticated(true);
}