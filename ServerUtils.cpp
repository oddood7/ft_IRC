/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:17:19 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/15 12:15:32 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::commandInit()
{
	//_commandFunctions["CREATE"] = &Server::createChannel;
	_commandFunctions["JOIN"] = &Server::join;
	_commandFunctions["CAP"] = &Server::cap;
	_commandFunctions["PART"] = &Server::part;
	_commandFunctions["PASS"] = &Server::pass;
	_commandFunctions["NICK"] = &Server::nick;
	_commandFunctions["QUIT"] = &Server::quit;
	_commandFunctions["PING"] = &Server::ping;
	_commandFunctions["PRIVMSG"] = &Server::msg;
	_commandFunctions["USER"] = &Server::user;
	// _commandFunctions["OPER"] = &Server::becomeOper;
	_commandFunctions["KICK"] = &Server::kick;
	_commandFunctions["INVITE"] = &Server::invite;
	_commandFunctions["TOPIC"] = &Server::topic;
	// _commandFunctions["MODE"] = &Server::setMode;
}

Channel &Server::getUserChannel(std::string channel)
{
	for (size_t i = 0; i < _channelsList.size(); i++) {
		if (channel == _channelsList[i].getName())
			return _channelsList[i];
	}
	return _channelsList[0];
}

void	Server::useCommand(User &user)
{
	std::map<std::string, CommandFunction> ::iterator it = _commandFunctions.find(ft_toupper(user.getBuf()[0]));
	if (it != _commandFunctions.end())
		(this->*(it->second))(user);
	else
		std::cerr << RED << ERR_UNKNOWNCOMMAND(user.getBuf()[0]) << RESET << std::endl;
}

void	Server::deleteFromChannel(User &user)
{
	for (size_t i = 0; i < _channelsList.size(); i++)
	{
		for (size_t j = 0; j < _channelsList[i].getChatters().size(); j++)
		{
			if (user.getNickName() == _channelsList[i].getChatters()[j])
			{
				std::cout << _channelsList[i].getChatters()[j] << std::endl;
				_channelsList[i].getChatters().erase(_channelsList[i].getChatters().begin() + j);
				j--;
			}
		}
	}
}

std::string	Server::ft_toupper(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(str[i]);
	return (str);	
}

void	Server::closeSocket()
{
	for (int i = 0; i <= _activeUsers; i++)
		close(_fds[i].fd);
}

int	Server::isInChannel(User &user, std::string channel)
{
	if (channel != user.getChannel())
	{
		std::string err = ERR_NOTONCHANNEL(channel);
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return 1;
	}
	return 0;
}

void	Server::send_all(Channel &channel, std::string rpl, std::string user)
{
	for (size_t i = 0; i < channel.getChatters().size(); i++)
	{
		for (size_t j = 0; j < usersManage.size(); j++)
		{
			if (channel.getChatters()[i] == usersManage[j].getNickName() && user != channel.getChatters()[i])
				send(usersManage[j].getSocket(), rpl.c_str(), rpl.size(), 0);
		}
	}
}

int	Server::is_op(User &user, std::string nickname)
{
	if (getUserChannel(user.getChannel()).getOwner() == nickname)
		return 1;
	for (size_t i = 0; i < getUserChannel(user.getChannel()).getOperator().size(); i++){
		if (getUserChannel(user.getChannel()).getOperator()[i] == nickname)
				return 1;
	}
	return -1;
}

void    Server::ping(User& user) {
    
    std::string pingu = "PONG :localhost\r\n";
    send(user.getSocket(), pingu.c_str(), pingu.size(), 0);
    std::cout << user_id(user.getUserName(), user.getNickName()) << " " << pingu;
}

void Server::SignalHandler(int sig)
{
	(void)sig;
	std::cout << std::endl << "signal sent." << std::endl;
	Server::signal = true; //  to stop the server
}