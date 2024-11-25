/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:21:16 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 15:15:44 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cctype>
# include <unistd.h>
# include <algorithm>
# include <cstdlib>
# include <cstring>
# include <csignal>
# include <ctime>
# include <poll.h>
# include <vector>
# include <map>
# include <sstream>
# include "User.hpp"
# include "Channel.hpp"
# include "Utils.hpp"

# define MAX_USERS 5

class Server
{
		private:
		
		Server();

		int	_socket;
		int	_port;
		int _pollRet;
		int _activeUsers;
		int _activeChannels;

		std::string _name;
		std::string	_password;

		sockaddr_in _address;
		socklen_t _size;
		pollfd _fds[MAX_USERS + 1];

		typedef void (Server::*CommandFunction)(User&);
		std::map<std::string, CommandFunction> _commandFunctions;
		std::map<int, std::string> _partialCommands;
		std::map<int, User> usersManage;
		std::vector<Channel> _channelsList;

		bool	_shutdown;
		static bool	signal;

		public:
		
		~Server();
		Server(int port, std::string pass);

		void	run();
		void	closeSocket();
		void	createUser();
		void	deleteUser(User &user);
		void	listenUser();
		void	commandInit();
		void	useCommand(User &user);

		void	cap(User &user);
		void	nick(User &user);
		void	user(User &user);
		void	pass(User &user);
		void	ping(User &user);
		void	msg(User &user);
		void	quit(User &user);
		void	join(User &user);
		void	mode(User &user);
		void	topic(User &user);
		void	part(User &user);
		void	invite(User &user);
		void	kick(User &user);

		void	nickIrssi(User &user, int i);
		void	userIrssi(User &user, int i);
		bool	passIrssi(User &user, int i);

		void    modeInvite(User &user);
		void	modeTopic(User &user);
		int 	modeKey(User &user);
		int		modeMaxUser(User &user);
		int 	operatorMode(User &user);
		std::string	ft_toupper(std::string str);
		int		searchUserChannel(User &user);
		void 	deleteChannel(const std::string& channelName);
		User*		getUserByNickname(const std::string &nickname);
		int		is_op(User &user, std::string nickname);
		int		isInChannel(User &user, std::string channel);
		void	deleteFromChannel(User &user);
		Channel &getUserChannel(const std::string& channelName);
		void	send_all(Channel &channel, std::string rpl, std::string user);
		static void 	SignalHandler(int sig);
		void 	processCommands(User &user, std::string &buffer);
		bool 	isUserInChannel(const std::string& nickname, const std::string& channelName);
		void	who(User &user);


};

#endif