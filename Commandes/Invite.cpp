/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:00:13 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/17 16:38:56 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

void debugPrintChannelUsers2(Channel& channel) {
    std::cout << "=== Debug: Channel " << channel.getName() << " ===" << std::endl;
    
    std::cout << "Invited users:" << std::endl;
    const std::vector<std::string>& invitedUsers = channel.getInvite();
    for (std::vector<std::string>::const_iterator it = invitedUsers.begin(); it != invitedUsers.end(); ++it) {
        std::cout << "  - " << *it << std::endl;
    }
    
    std::cout << "Chatters:" << std::endl;
    const std::vector<std::string>& chatters = channel.getChatters();
    for (std::vector<std::string>::const_iterator it = chatters.begin(); it != chatters.end(); ++it) {
        std::cout << "  - " << *it << std::endl;
    }
    
    std::cout << "===============================" << std::endl;
}

void Server::invite(User &user)
{
    std::string err;
    if (user.getBuf().size() < 3) {
        err = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[2];
    std::string invitedNickname = user.getBuf()[1];

    if (isInChannel(user, channelName) != 0) {
		std::cout << "hello";
        return; // isInChannel envoie déjà le message d'erreur
    }
    if (is_op(user, user.getNickName()) == -1) {
		std::cout << "hello2";
        err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    User* invitedUser = NULL;
    for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it) {
        if (it->second.getNickName() == invitedNickname) {
            invitedUser = &(it->second);
            break;
        }
    }
    if (!invitedUser) {
		std::cout << "hello3";
        err = ERR_NOSUCHNICK(invitedNickname);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    Channel &channel = getUserChannel(channelName);

	std::cout << "Before INVITE:" << std::endl;
    debugPrintChannelUsers2(channel);
	
    std::vector<std::string> &chatters = channel.getChatters();
    bool userAlreadyInChannel = false;
    for (std::vector<std::string>::const_iterator it = chatters.begin(); it != chatters.end(); ++it) {
        if (*it == invitedNickname) {
			std::cout << "hello4";
            userAlreadyInChannel = true;
            break;
        }
    }
    if (userAlreadyInChannel) {
		std::cout << "hello5";
        err = ERR_USERONCHANNEL(invitedNickname, channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    channel.getInvite().push_back(invitedNickname);
    std::string inviteMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " INVITE " + invitedNickname + " :" + channelName + "\r\n";
    send(invitedUser->getSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
    err = RPL_INVITING(channelName, invitedNickname);
    send(user.getSocket(), err.c_str(), err.size(), 0);

	std::cout << "After INVITE:" << std::endl;
    debugPrintChannelUsers2(channel);
}