/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diavolo <diavolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:45:38 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/29 13:13:58 by diavolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <unistd.h>
# include <fstream>
# include <cstdlib>
# include <cctype>
# include <cstring>
# include <csignal>
# include <sys/socket.h>
# include <sys/types.h>
# include <sstream>
# include <poll.h>
# include <netinet/in.h>
#include <arpa/inet.h>
# include <stdexcept>
#include <map>
#include <vector>
#include "Client.hpp"

class Client;

class Server
{
	private :

	int	_port;
	int	_serverSocket;
	char _buff[1024];
	int _bytesRead;
	unsigned int _client_number;
	//int	_channels;
	// std::string _name;
	// std::string _password;
	//bool _stop;

	sockaddr_in _serverAddr;
	std::map<std::vector<std::string const>, class Client> map_client;
	std::vector<std::string const> vec_client;
	pollfd fds[1];
	Server();
	
	public :
	int getServerSocket(){
		return this->_serverSocket;
	};

	Server(int port); //, std::string pswd);
	~Server();

	void	run();
	// void    socketCloser();
	// void	createClient();
	// void	listenClient();
};

#endif