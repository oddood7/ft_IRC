/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:00:13 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/23 15:16:10 by lde-mais         ###   ########.fr       */
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

    std::string invitedNickname = user.getBuf()[1];
    std::string channelName = user.getBuf()[2];

    Channel* channel = NULL;
    for (size_t i = 0; i < _channelsList.size(); i++) {
        if (_channelsList[i].getName() == channelName) {
            channel = &_channelsList[i];
            break;
        }
    }

    if (!channel) {
        err = ERR_NOSUCHCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    if (std::find(channel->getChatters().begin(), channel->getChatters().end(), user.getNickName()) == channel->getChatters().end()) {
        err = ERR_NOTONCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    if (is_op(user, user.getNickName()) == -1) {
        err = ERR_CHANOPRIVSNEEDED(channelName);
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

    if (std::find(channel->getChatters().begin(), channel->getChatters().end(), invitedNickname) != channel->getChatters().end()) {
        err = ERR_USERONCHANNEL(invitedNickname, channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    channel->getInvite().push_back(invitedNickname);
    std::string inviteMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " INVITE " + invitedNickname + " :" + channelName + "\r\n";
    send(invitedUser->getSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
    err = RPL_INVITING(channelName, invitedNickname);
    send(user.getSocket(), err.c_str(), err.size(), 0);
}