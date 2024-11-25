/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:09:56 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/23 15:17:17 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

void Server::kick(User &user)
{
    std::string err;
    if (user.getBuf().size() < 4) {
        err = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[1];
    Channel &channel = getUserChannel(channelName);

    if (channel.getName().empty()) {
        err = ERR_NOSUCHCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    if (isInChannel(user, channelName)) {
        return;
    }

    if (is_op(user, user.getNickName()) == -1) {
        err = ERR_CHANOPRIVSNEEDED(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string kickedNick = user.getBuf()[2];
    std::string reason = user.getBuf().size() > 3 ? user.getBuf()[3] : "No reason specified";
    
    bool userFound = false;
    for (size_t i = 0; i < channel.getChatters().size(); ++i) {
        if (kickedNick == channel.getChatters()[i]) {
            userFound = true;
            break;
        }
    }

    if (!userFound) {
        err = ERR_USERNOTINCHANNEL(kickedNick, channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string kickMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + 
                          " KICK " + channelName + " " + kickedNick + " :" + reason + "\r\n";

    for (size_t i = 0; i < channel.getChatters().size(); ++i) {
        User* chatUser = getUserByNickname(channel.getChatters()[i]);
        if (chatUser) {
            send(chatUser->getSocket(), kickMsg.c_str(), kickMsg.size(), 0);
        }
    }

    User* kickedUser = getUserByNickname(kickedNick);
    if (kickedUser) {
        kickedUser->getChannel().clear();
    }
    
    for (std::vector<std::string>::iterator it = channel.getChatters().begin(); it != channel.getChatters().end(); ++it) {
        if (*it == kickedNick) {
            channel.getChatters().erase(it);
            break;
        }
    }
}