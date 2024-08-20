/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:11:08 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/20 16:11:09 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::deleteChannel(const std::string& channelName)
{
    for (size_t i = 0; i < _channelsList.size(); ++i)
    {
        if (_channelsList[i].getName() == channelName)
        {
            _channelsList.erase(_channelsList.begin() + i);
            break;
        }
    }
}

User* Server::getUserByNickname(const std::string &nickname)
{
    for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it)
    {
        if (it->second.getNickName() == nickname)
        {
            return &(it->second);
        }
    }
    return NULL;
}

void Server::part(User &user)
{
    if (user.getBuf().size() < 2) {
        std::string err = ERR_NEEDMOREPARAMS(user.getHost(), "PART");
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[1];
    std::string reason = (user.getBuf().size() > 2) ? user.getBuf()[2] : "Leaving";

    Channel* channel = NULL;
    for (size_t i = 0; i < _channelsList.size(); ++i) {
        if (_channelsList[i].getName() == channelName) {
            channel = &_channelsList[i];
            break;
        }
    }

    if (!channel) {
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    bool userInChannel = false;
    for (size_t i = 0; i < channel->getChatters().size(); ++i) {
        if (channel->getChatters()[i] == user.getNickName()) {
            userInChannel = true;
            channel->getChatters().erase(channel->getChatters().begin() + i);
            break;
        }
    }

    if (!userInChannel) {
        std::string err = ERR_NOTONCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    std::string partMessage = RPL_PART(user_id(user.getUserName(), user.getNickName()), channelName, reason);
    
    for (size_t i = 0; i < channel->getChatters().size(); ++i) {
        User* chatUser = getUserByNickname(channel->getChatters()[i]);
        if (chatUser) {
            send(chatUser->getSocket(), partMessage.c_str(), partMessage.size(), 0);
        }
    }
    send(user.getSocket(), partMessage.c_str(), partMessage.size(), 0);

    Channel emptyChannel;
    user.setChannel(emptyChannel);

 	if (channel->getChatters().empty()) {
        for (int i = 0; i < _activeChannels; i++) {
            if (_channelsList[i].getName() == channelName) {
                _channelsList.erase(_channelsList.begin() + i);
                _activeChannels--;
                channel = NULL;
                break;
            }
        }
    }
    std::string closeMessage = ":" + _name + " KICK " + channelName + " " + user.getNickName() + " :Left the channel\r\n";
    send(user.getSocket(), closeMessage.c_str(), closeMessage.size(), 0);
}
