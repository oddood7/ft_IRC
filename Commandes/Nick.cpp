/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:08:48 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/15 14:13:12 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

void Server::nick(User &user)
{
    if (user.getBuf().size() == 1) {
        std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string newNick = user.getBuf()[1];
    std::string oldNick = user.getNickName();

    if (newNick.size() > 9) {
        newNick = newNick.substr(0, 9);
    }

    for (size_t i = 0; i < newNick.size(); i++) {
        if (!isalnum(newNick[i])) {
            std::string err = ERR_ERRONEUSNICKNAME(_name, newNick);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
    }

    for (std::map<int, User>::iterator ite = usersManage.begin(); ite != usersManage.end(); ite++) {
        if (newNick == ite->second.getNickName()) {
            std::string err = ERR_NICKNAMEINUSE(_name, newNick);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
    }
    user.setNickName(newNick);

    std::string nickChangeMsg = ":" + oldNick + "!" + user.getUserName() + "@" + user.getHost() + " NICK :" + newNick + "\r\n";

    for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it) {
        send(it->second.getSocket(), nickChangeMsg.c_str(), nickChangeMsg.size(), 0);
    }

    for (size_t j = 0; j < _channelsList.size(); ++j) {
        std::vector<std::string>& chatters = _channelsList[j].getChatters();
        for (size_t k = 0; k < chatters.size(); ++k) {
            if (chatters[k] == oldNick) {
                chatters[k] = newNick;
                break;
            }
        }
    }

    std::cout << "Nickname changed from " << oldNick << " to " << newNick << std::endl;
}

void Server::nickIrssi(User &user, int i)
{
    if (user.getBuf().size() <= (size_t)i + 1) {
        std::string err = ERR_NONICKNAMEGIVEN(user.getHost());
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string newNick = user.getBuf()[i + 1];
    std::string oldNick = user.getNickName();

    for (std::map<int, User>::iterator ite = usersManage.begin(); ite != usersManage.end(); ++ite)
    {
        if (ite->second.getNickName() == newNick) {
            std::stringstream ss;
            ss << _activeUsers;
            newNick += ss.str();
            break;
        }
    }

    user.setNickName(newNick);

    std::string nickChangeMsg = RPL_NICK(oldNick, user.getUserName(), newNick);
	if (user.getIrssi()) {
    	std::string irssiUpdate = ":" + _name + " 004 " + newNick + " " + _name + " ircserv-1.0 DOQRSZaghilopswz CFILMPQSbcefgijklmnopqrstvz bkloveqjfI\r\n";
    	send(user.getSocket(), irssiUpdate.c_str(), irssiUpdate.size(), 0);
	}	
    
    for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it)
    {
        send(it->second.getSocket(), nickChangeMsg.c_str(), nickChangeMsg.size(), 0);
    }

    for (size_t j = 0; j < _channelsList.size(); ++j)
    {
        std::vector<std::string>& chatters = _channelsList[j].getChatters();
        for (size_t k = 0; k < chatters.size(); ++k)
        {
            if (chatters[k] == oldNick)
            {
                chatters[k] = newNick;
                break;
            }
        }
    }
}
