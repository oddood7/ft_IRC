/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:00:13 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/20 16:08:51 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

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
        return; // isInChannel envoie déjà le message d'erreur
    }
    if (is_op(user, user.getNickName()) == -1) {
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
        err = ERR_NOSUCHNICK(invitedNickname);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    Channel &channel = getUserChannel(channelName);
    std::vector<std::string> &chatters = channel.getChatters();
    bool userAlreadyInChannel = false;
    for (std::vector<std::string>::const_iterator it = chatters.begin(); it != chatters.end(); ++it) {
        if (*it == invitedNickname) {
            userAlreadyInChannel = true;
            break;
        }
    }
    if (userAlreadyInChannel) {
        err = ERR_USERONCHANNEL(invitedNickname, channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    channel.getInvite().push_back(invitedNickname);
    std::string inviteMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " INVITE " + invitedNickname + " :" + channelName + "\r\n";
    send(invitedUser->getSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
    err = RPL_INVITING(channelName, invitedNickname);
    send(user.getSocket(), err.c_str(), err.size(), 0);
}