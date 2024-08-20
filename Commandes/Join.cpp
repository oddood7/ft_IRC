/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:53:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/20 16:09:13 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Channel.hpp"

int		check_chanpswd(User &user, Channel &channel)
{
	if (!channel.getPass().empty())
	{
		if (!user.getBuf()[2].empty())
		{
			if (user.getBuf()[2] == channel.getPass())
				return 0;
			else
				return 1;
		}
		return 1;
	}
	return 0;
}

void	joinChannel(User &user, Channel &channel)
{
	if (channel.getL() != 0 && channel.getChatters().size() >= channel.getL()) {
		std::string err = ERR_CHANNELISFULL(channel.getName());
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	if (channel.getI() == true)
	{
		size_t i;
		for	(i = 0; i < channel.getInvite().size(); i++)
		{
			if (user.getNickName() == channel.getInvite()[i]) {
				break ;
			}
		}
		if (i == channel.getInvite().size()) {
			std::string err = ERR_INVITEONLYCHAN(channel.getName());
			send(user.getSocket(), err.c_str(), err.size(), 0);
			return ;
		}

	}
	if (check_chanpswd(user, channel)) {
		std::string err = ERR_BADCHANNELKEY(channel.getName());
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	channel.getChatters().push_back(user.getNickName());
	std::string rpl;
	if (channel.getTopic().size() > 0)
		rpl = RPL_TOPIC(channel.getName(), channel.getTopic());
	else
		rpl = RPL_NOTOPIC(channel.getName());
	user.setChannel(channel);
	send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
}

void Server::join(User& user)
{
    if (user.getBuf().size() == 1)
    {
        std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[1];
    std::string password = (user.getBuf().size() > 2) ? user.getBuf()[2] : "";

    if (channelName[0] != '#' && channelName[0] != '&' && channelName[0] != '1')
        channelName.insert(0, "#");

    if (channelName.find(',') != std::string::npos || channelName.find(' ') != std::string::npos){
        std::cerr << RED << (channelName) << RESET << std::endl;
        return;
    }
    Channel* channel = NULL;
    for (int i = 0; i < _activeChannels; i++)
    {
        if (channelName == _channelsList[i].getName())
        {
            channel = &_channelsList[i];
            break;
        }
    }

    if (channel)
    {
        if (user.getBuf().size() > 3) {
            std::string err = ERR_BADCHANNELKEY(channel->getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
        joinChannel(user, *channel);
    }
    else
    {
        if (user.getBuf().size() > 3) {
            std::string err = ERR_BADPASS(_name);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
        if (channelName.size() > 32)
            channelName = channelName.substr(0, 32);
        _channelsList.push_back(Channel(channelName, user.getNickName()));
        _activeChannels++;
        joinChannel(user, _channelsList[_channelsList.size() - 1]);
    }
    std::string joinMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " JOIN " + channelName + "\r\n";
    send(user.getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
}
