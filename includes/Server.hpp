/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:45:38 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/22 16:00:43 by lde-mais         ###   ########.fr       */
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
# include <stdexcept>

# define MAX_CLIENTS 7
class Server
{
	private :

	int	_port;
	int	_socket;
	//int	_channels;
	// std::string _name;
	// std::string _password;
	//bool _stop;

	sockaddr_in _address;
	Server();
	
	public :

	Server(int port); //, std::string pswd);
	~Server();

	void	run();
	// void    socketCloser();
	// void	createClient();
	// void	listenClient();
};

#endif