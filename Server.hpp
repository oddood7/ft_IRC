/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:45:38 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/20 12:32:49 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
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

class Server
{
	private :

	int	_port;
	int	_socket;
	std::string _name;
	std::string _password;
	bool _stop;

	sockaddr_in _address;
	socklen_t _size;
	Server();
	
	public :

	Server(int port, std::string pswd);
	~Server();

	void	run();
};

#endif