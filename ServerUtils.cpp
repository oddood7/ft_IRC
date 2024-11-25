/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:17:19 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 15:19:42 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::commandInit()
{
	_commandFunctions["JOIN"] = &Server::join;
	_commandFunctions["CAP"] = &Server::cap;
	_commandFunctions["PART"] = &Server::part;
	_commandFunctions["PASS"] = &Server::pass;
	_commandFunctions["NICK"] = &Server::nick;
	_commandFunctions["QUIT"] = &Server::quit;
	_commandFunctions["PING"] = &Server::ping;
	_commandFunctions["PRIVMSG"] = &Server::msg;
	_commandFunctions["USER"] = &Server::user;
	_commandFunctions["KICK"] = &Server::kick;
	_commandFunctions["INVITE"] = &Server::invite;
	_commandFunctions["TOPIC"] = &Server::topic;
	_commandFunctions["MODE"] = &Server::mode;
	_commandFunctions["WHO"] = &Server::who;
}

bool Server::isUserInChannel(const std::string& nickname, const std::string& channelName) {
    for (size_t i = 0; i < _channelsList.size(); ++i) {
        if (_channelsList[i].getName() == channelName) {
            return _channelsList[i].isChatter(nickname);
        }
    }
    return false;
}

Channel &Server::getUserChannel(const std::string& channelName)
{
    // Vérifier si le nom du canal est vide
    if (channelName.empty()) {
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        throw std::runtime_error(err);
    }

    // Vérifier si le canal existe
    for (size_t i = 0; i < _channelsList.size(); i++) {
        if (channelName == _channelsList[i].getName()) {
            return _channelsList[i];
        }
    }

    // Si le canal n'existe pas, créer un nouveau canal
    if (_activeChannels == 0 || _channelsList.empty()) {
        _channelsList.push_back(Channel(channelName, ""));
        _activeChannels++;
        return _channelsList[_channelsList.size() - 1];
    }

    // Si nous ne pouvons pas créer un nouveau canal, retourner le premier canal
    // avec un message d'erreur
    std::string err = ERR_NOSUCHCHANNEL(channelName);
    std::cout << RED << err << RESET << std::endl;
    return _channelsList[0];
}

// void	Server::useCommand(User &user)
// {
// 	std::map<std::string, CommandFunction> ::iterator it = _commandFunctions.find(ft_toupper(user.getBuf()[0]));
// 	if (it != _commandFunctions.end())
// 		(this->*(it->second))(user);
// 	else
// 		std::cerr << RED << ERR_UNKNOWNCOMMAND(user.getBuf()[0]) << RESET << std::endl;
// }

void Server::useCommand(User &user)
{
    if (user.getBuf().empty())
        return;
    
    std::string command = ft_toupper(user.getBuf()[0]);

    // Traitement spécial pour CAP
    if (command == "CAP")
    {
        cap(user);
        return;
    }
    
    std::map<std::string, CommandFunction>::iterator it = _commandFunctions.find(command);
    if (it != _commandFunctions.end())
    {
        // Si c'est une commande NICK et que l'utilisateur est déjà vérifié
        if (command == "NICK" && user.getVerif() && user.getIrssi())
        {
            std::string oldNick = user.getNickName();
            bool wasInChannel = false;
            std::string currentChannel;
            
            // Vérifier si l'utilisateur est dans un channel
            for (size_t i = 0; i < _channelsList.size(); ++i)
            {
                std::vector<std::string>& chatters = _channelsList[i].getChatters();
                for (size_t j = 0; j < chatters.size(); ++j)
                {
                    if (chatters[j] == oldNick)
                    {
                        wasInChannel = true;
                        currentChannel = _channelsList[i].getName();
                        chatters.erase(chatters.begin() + j);
                        break;
                    }
                }
                if (wasInChannel)
                    break;
            }
            
            // Exécuter la commande NICK
            (this->*(it->second))(user);
            
            // Si l'utilisateur était dans un channel, le remettre avec son nouveau nickname
            if (wasInChannel)
            {
                for (size_t i = 0; i < _channelsList.size(); ++i)
                {
                    if (_channelsList[i].getName() == currentChannel)
                    {
                        _channelsList[i].getChatters().push_back(user.getNickName());
                        break;
                    }
                }
            }
        }
        else
        {
            // Exécuter la commande normalement
            (this->*(it->second))(user);
        }
    }
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
		std::cout << "try";
		std::string err = ERR_NOTONCHANNEL(channel);
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return 1;
	}
	return 0;
}

void	Server::send_all(Channel &channel, std::string rpl, std::string user)
{
	if (rpl.empty()) {
        std::cerr << RED << "Error: Message to send is empty" << RESET << std::endl;
        return;
    }
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
	std::cout << std::endl << "Signal Recieved ! Terminating MYIRC!" << std::endl;
	Server::signal = true; //  to stop the server
}

void Server::who(User &user)
{
    // Si pas de paramètres, on envoie juste la fin de la liste
    if (user.getBuf().size() < 2)
    {
        std::string endWho = ":" + _name + " 315 " + user.getNickName() + " * :End of WHO list\r\n";
        send(user.getSocket(), endWho.c_str(), endWho.size(), 0);
        return;
    }

    std::string target = user.getBuf()[1];
    
    // Si c'est une recherche sur un channel
    if (target[0] == '#')
    {
        for (size_t i = 0; i < _channelsList.size(); ++i)
        {
            if (_channelsList[i].getName() == target)
            {
                std::vector<std::string>& chatters = _channelsList[i].getChatters();
                for (size_t j = 0; j < chatters.size(); ++j)
                {
                    User* member = getUserByNickname(chatters[j]);
                    if (member)
                    {
                        // RPL_WHOREPLY format : "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
                        std::string whoReply = ":" + _name + " 352 " + user.getNickName() + " " +
                                             target + " " +
                                             member->getUserName() + " " +
                                             member->getHost() + " " +
                                             _name + " " +
                                             member->getNickName() + " H :0 " +
                                             member->getUserName() + "\r\n";
                        send(user.getSocket(), whoReply.c_str(), whoReply.size(), 0);
                    }
                }
                break;
            }
        }
    }

    // Envoyer la fin de la liste WHO
    std::string endWho = ":" + _name + " 315 " + user.getNickName() + " " + target + " :End of WHO list\r\n";
    send(user.getSocket(), endWho.c_str(), endWho.size(), 0);
}