/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:53:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/21 13:07:50 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Channel.hpp"

void debugPrintChannelUsers(Channel& channel) {
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
		std::cout << "dans join" << std::endl;
		size_t i;
		for	(i = 0; i < channel.getInvite().size(); i++)
		{
			if (user.getNickName() == channel.getInvite()[i]) {
				break ;
			}
			std::cout << "dans for" << std::endl;
		}
		if (i == channel.getInvite().size()) {
			std::cout << "a la fin" << std::endl;
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

// void Server::join(User& user)
// {
// 	std::cout << "\n=== JOIN command received ===" << std::endl;
//     std::cout << "User: " << user.getNickName() << std::endl;
//     std::cout << "Buffer size: " << user.getBuf().size() << std::endl;
//     if (user.getBuf().size() == 1)
//     {
//         std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::string channelName = user.getBuf()[1];
//     std::string password = (user.getBuf().size() > 2) ? user.getBuf()[2] : "";

// 	std::cout << "Channel name: " << channelName << std::endl;
//     std::cout << "Password provided: " << (password.empty() ? "No" : "Yes") << std::endl;
//     if (channelName[0] != '#' && channelName[0] != '&' && channelName[0] != '1') {
//         channelName.insert(0, "#");
// 		std::cout << "Added # to channel name. New name: " << channelName << std::endl;
// 	}

//     if (channelName.find(',') != std::string::npos || channelName.find(' ') != std::string::npos){
//         std::cerr << RED << (channelName) << RESET << std::endl;
//         return;
//     }
// 	std::cout << "Searching for channel: " << channelName << std::endl;
//     std::cout << "Number of active channels: " << _activeChannels << std::endl;
//     Channel* channel = NULL;
//     for (int i = 0; i < _activeChannels; i++)
//     {
// 		std::cout << "Checking channel: " << _channelsList[i].getName() << std::endl;
//         if (channelName == _channelsList[i].getName())
//         {
// 			std::cout << "Channel found: " << channelName << std::endl;
//             channel = &_channelsList[i];
//             break;
//         }
// 		if (channel == NULL) {
// 			std::cout << "Channel not found: " << channelName << std::endl;
// 		}
//     }

//     if (channel)
//     {
// 		std::cout << "Before JOIN:" << std::endl;
//         debugPrintChannelUsers(*channel);
//         if (user.getBuf().size() > 3) {
//             std::string err = ERR_BADCHANNELKEY(channel->getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }
//         joinChannel(user, *channel);
// 		std::cout << "after JOIN:" << std::endl;
//         debugPrintChannelUsers(*channel);
//     }
//     else
//     {
// 		std::cout << "Channel does not exist. Creating new channel." << std::endl;
//         if (user.getBuf().size() > 3) {
//             std::string err = ERR_BADPASS(_name);
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }
//         if (channelName.size() > 32)
//             channelName = channelName.substr(0, 32);
//         _channelsList.push_back(Channel(channelName, user.getNickName()));
//         _activeChannels++;
// 		std::cout << "New channel created. Active channels: " << _activeChannels << std::endl;
//         joinChannel(user, _channelsList[_channelsList.size() - 1]);
//     }
//     std::string joinMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " JOIN " + channelName + "\r\n";
//     send(user.getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
// 	std::cout << "Join message sent: " << joinMessage << std::endl;
//     std::cout << "=== END OF JOIN command ===" << std::endl;
// }

void Server::join(User& user)
{
    if (user.getBuf().size() < 2)
    {
        std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[1];
    std::string password = (user.getBuf().size() > 2) ? user.getBuf()[2] : "";

    if (channelName[0] != '#' && channelName[0] != '&') {
        channelName = "#" + channelName;
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

    bool newChannel = false;
    if (!channel)
    {
        if (channelName.size() > 32)
            channelName = channelName.substr(0, 32);
        _channelsList.push_back(Channel(channelName, user.getNickName()));
        channel = &_channelsList[_channelsList.size() - 1];
        _activeChannels++;
        newChannel = true;
    }
    else
    {
        if (channel->getI() && std::find(channel->getInvite().begin(), channel->getInvite().end(), user.getNickName()) == channel->getInvite().end()) {
            std::string err = ERR_INVITEONLYCHAN(channel->getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }

        if (!channel->getPass().empty() && channel->getPass() != password) {
            std::string err = ERR_BADCHANNELKEY(channel->getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }

        if (channel->getL() != 0 && channel->getChatters().size() >= channel->getL()) {
            std::string err = ERR_CHANNELISFULL(channel->getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
    }

    // Ajouter l'utilisateur au canal
    channel->getChatters().push_back(user.getNickName());
    user.setChannel(*channel);

    // Envoyer le message JOIN à tous les utilisateurs du canal, y compris celui qui vient de rejoindre
    std::string joinMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " JOIN " + channelName + "\r\n";
    for (std::vector<std::string>::const_iterator it = channel->getChatters().begin(); it != channel->getChatters().end(); ++it) {
        User* channelUser = getUserByNickname(*it);
        if (channelUser) {
            send(channelUser->getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
        }
    }

    // Envoyer le topic
    std::string rpl;
    if (!channel->getTopic().empty())
        rpl = RPL_TOPIC(channel->getName(), channel->getTopic());
    else
        rpl = RPL_NOTOPIC(channel->getName());
    send(user.getSocket(), rpl.c_str(), rpl.size(), 0);

    // Envoyer la liste des utilisateurs du canal
    std::string userList = ":" + _name + " 353 " + user.getNickName() + " = " + channelName + " :";
    for (std::vector<std::string>::const_iterator it = channel->getChatters().begin(); it != channel->getChatters().end(); ++it) {
        userList += *it + " ";
    }
    userList += "\r\n";
    send(user.getSocket(), userList.c_str(), userList.size(), 0);

    std::string endOfList = ":" + _name + " 366 " + user.getNickName() + " " + channelName + " :End of /NAMES list.\r\n";
    send(user.getSocket(), endOfList.c_str(), endOfList.size(), 0);

    // Si c'est un nouveau canal, nous ne définissons plus l'utilisateur comme opérateur automatiquement
    if (newChannel) {
        // Vous pouvez ajouter ici d'autres initialisations pour un nouveau canal si nécessaire
    }
}