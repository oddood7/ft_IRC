/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:30:32 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/04 15:24:40 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Channel.hpp"
# include "User.hpp"
# include "Utils.hpp"
# include <cstdlib>
# include <cstring>
# include <iostream>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include <stdexcept>
# include <unistd.h>

# define MAX_USERS 5

class Server
{
  private:
	int _port;
	int _socket;
	int _activeUsers;
	std::string _password;
	bool _shutdown;
	struct sockaddr_in _address;
	struct pollfd _fds[MAX_USERS + 1];
	std::map<int, User> UsersManage;

	void acceptNewConnection();
	void processUserData(int UserIndex);
	bool authenticateUser(const std::string &password)
	{
		return (password == _password);
	}
	void createChannel(User &user, const std::string &channelName);
	void joinChannel(User &user, const std::string &channelName);
	void partChannel(User &user, const std::string &channelName);
	void login(User &user, const std::string &password);
	void setNick(User &user, const std::string &nick);
	void setUser(User &user, const std::string &userInfo);
	typedef void (Server::*CommandFunction)(User &, const std::string &);
	std::map<std::string, CommandFunction> _commandFunctions;

  public:
	Server(int port, const std::string &password);
	~Server();
	void run();
	int getUserIndex(int fd);
};

#endif
