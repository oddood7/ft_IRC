/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:53:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 15:21:21 by lde-mais         ###   ########.fr       */
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


// void Server::join(User& user)
// {
//     if (user.getBuf().size() < 2)
//     {
//         std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::string channelName = user.getBuf()[1];
//     std::string password = (user.getBuf().size() > 2) ? user.getBuf()[2] : "";

//     if (channelName[0] != '#' && channelName[0] != '&') {
//         channelName = "#" + channelName;
//     }

//     Channel* channel = NULL;
//     for (int i = 0; i < _activeChannels; i++)
//     {
//         if (channelName == _channelsList[i].getName())
//         {
//             channel = &_channelsList[i];
//             break;
//         }
//     }

//     bool newChannel = false;
//     if (!channel)
//     {
//         if (channelName.size() > 32)
//             channelName = channelName.substr(0, 32);
//         _channelsList.push_back(Channel(channelName, user.getNickName()));
//         channel = &_channelsList[_channelsList.size() - 1];
//         _activeChannels++;
//         newChannel = true;
//     }
//     else
//     {
//         if (channel->getI() && std::find(channel->getInvite().begin(), channel->getInvite().end(), user.getNickName()) == channel->getInvite().end()) {
//             std::string err = ERR_INVITEONLYCHAN(channel->getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }

//         if (!channel->getPass().empty() && channel->getPass() != password) {
//             std::string err = ERR_BADCHANNELKEY(channel->getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }

//         if (channel->getL() != 0 && channel->getChatters().size() >= channel->getL()) {
//             std::string err = ERR_CHANNELISFULL(channel->getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }
//     }

//     channel->getChatters().push_back(user.getNickName());
//     user.setChannel(*channel);

//     std::string joinMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " JOIN " + channelName + "\r\n";
//     for (std::vector<std::string>::const_iterator it = channel->getChatters().begin(); it != channel->getChatters().end(); ++it) {
//         User* channelUser = getUserByNickname(*it);
//         if (channelUser) {
//             send(channelUser->getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
//         }
//     }

//     std::string rpl;
//     if (!channel->getTopic().empty())
//         rpl = RPL_TOPIC(channel->getName(), channel->getTopic());
//     else
//         rpl = RPL_NOTOPIC(channel->getName());
//     send(user.getSocket(), rpl.c_str(), rpl.size(), 0);

//     std::string userList = ":" + _name + " 353 " + user.getNickName() + " = " + channelName + " :";
//     for (std::vector<std::string>::const_iterator it = channel->getChatters().begin(); it != channel->getChatters().end(); ++it) {
//         userList += *it + " ";
//     }
//     userList += "\r\n";
//     send(user.getSocket(), userList.c_str(), userList.size(), 0);

//     std::string endOfList = ":" + _name + " 366 " + user.getNickName() + " " + channelName + " :End of /NAMES list.\r\n";
//     send(user.getSocket(), endOfList.c_str(), endOfList.size(), 0);

//     if (newChannel) {
//      }
// }

// void Server::join(User& user)
// {
//     if (user.getBuf().size() < 2)
//     {
//         std::string err = ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]);
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::string channelName = user.getBuf()[1];
//     std::string password = (user.getBuf().size() > 2) ? user.getBuf()[2] : "";
//     std::string nickname = user.getNickName();
//     std::string username = user.getUserName();
//     std::string hostname = user.getHost();

//     if (channelName[0] != '#' && channelName[0] != '&') {
//         channelName = "#" + channelName;
//     }

//     size_t channelIndex = _channelsList.size();
//     bool newChannel = true;

//     // Trouver ou créer le channel
//     for (size_t i = 0; i < _channelsList.size(); i++)
//     {
//         if (channelName == _channelsList[i].getName())
//         {
//             channelIndex = i;
//             newChannel = false;
//             break;
//         }
//     }

//     if (newChannel)
//     {
//         if (channelName.size() > 32)
//             channelName = channelName.substr(0, 32);
            
//         _channelsList.push_back(Channel(channelName, nickname));
//         channelIndex = _channelsList.size() - 1;
//         _channelsList[channelIndex].setI(false);
//         _activeChannels++;
//     }
//     else
//     {
//         // Vérifications du channel...
//         if (_channelsList[channelIndex].getI() && 
//             std::find(_channelsList[channelIndex].getInvite().begin(), 
//                      _channelsList[channelIndex].getInvite().end(), 
//                      nickname) == _channelsList[channelIndex].getInvite().end()) {
//             std::string err = ERR_INVITEONLYCHAN(channelName);
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }

//         if (!_channelsList[channelIndex].getPass().empty() && 
//             _channelsList[channelIndex].getPass() != password) {
//             std::string err = ERR_BADCHANNELKEY(channelName);
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }

//         if (_channelsList[channelIndex].getL() != 0 && 
//             _channelsList[channelIndex].getChatters().size() >= _channelsList[channelIndex].getL()) {
//             std::string err = ERR_CHANNELISFULL(channelName);
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return;
//         }

//         // Vérifier si l'utilisateur est déjà dans le channel sous un autre nickname
//         std::vector<std::string>& chatters = _channelsList[channelIndex].getChatters();
//         for (size_t i = 0; i < chatters.size(); ++i)
//         {
//             User* existingUser = getUserByNickname(chatters[i]);
//             if (existingUser && existingUser->getSocket() == user.getSocket())
//             {
//                 chatters.erase(chatters.begin() + i);
//                 break;
//             }
//         }
//     }

//     // Ajouter l'utilisateur au channel
//     _channelsList[channelIndex].getChatters().push_back(nickname);
//     user.setChannel(_channelsList[channelIndex]);

//     // Envoyer les messages
//     std::string joinMessage = ":" + nickname + "!" + username + "@" + hostname + " JOIN " + channelName + "\r\n";
//     const std::vector<std::string>& chatters = _channelsList[channelIndex].getChatters();
//     for (size_t i = 0; i < chatters.size(); ++i) {
//         User* channelUser = getUserByNickname(chatters[i]);
//         if (channelUser) {
//             send(channelUser->getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
//         }
//     }

//     // RPL_TOPIC
//     std::string rpl;
//     if (!_channelsList[channelIndex].getTopic().empty())
//         rpl = RPL_TOPIC(channelName, _channelsList[channelIndex].getTopic());
//     else
//         rpl = RPL_NOTOPIC(channelName);
//     send(user.getSocket(), rpl.c_str(), rpl.size(), 0);

//     // Liste des utilisateurs
//     std::string userList = ":" + _name + " 353 " + nickname + " = " + channelName + " :";
//     for (size_t i = 0; i < chatters.size(); ++i) {
//         userList += chatters[i] + " ";
//     }
//     userList += "\r\n";
//     send(user.getSocket(), userList.c_str(), userList.size(), 0);

//     std::string endOfList = ":" + _name + " 366 " + nickname + " " + channelName + " :End of /NAMES list.\r\n";
//     send(user.getSocket(), endOfList.c_str(), endOfList.size(), 0);
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
    std::string nickname = user.getNickName();
    std::string username = user.getUserName();
    std::string hostname = user.getHost();

    if (channelName[0] != '#' && channelName[0] != '&') {
        channelName = "#" + channelName;
    }

    size_t channelIndex = _channelsList.size();
    bool newChannel = true;

    for (size_t i = 0; i < _channelsList.size(); i++)
    {
        if (channelName == _channelsList[i].getName())
        {
            channelIndex = i;
            newChannel = false;
            break;
        }
    }

    if (newChannel)
    {
        if (channelName.size() > 32)
            channelName = channelName.substr(0, 32);
            
        Channel newChannel(channelName, nickname);
        newChannel.setI(false);  // Force le mode invite à false
        _channelsList.push_back(newChannel);
        channelIndex = _channelsList.size() - 1;
        _activeChannels++;

        // Envoie un message explicite de mode normal pour le nouveau channel
        std::string modeMsg = ":" + _name + " MODE " + channelName + " -i\r\n";
        send(user.getSocket(), modeMsg.c_str(), modeMsg.size(), 0);
    }
    else
    {
        if (_channelsList[channelIndex].getI() && 
            std::find(_channelsList[channelIndex].getInvite().begin(), 
                     _channelsList[channelIndex].getInvite().end(), 
                     nickname) == _channelsList[channelIndex].getInvite().end()) 
        {
            std::string err = ERR_INVITEONLYCHAN(channelName);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }

        if (!_channelsList[channelIndex].getPass().empty() && 
            _channelsList[channelIndex].getPass() != password) 
        {
            std::string err = ERR_BADCHANNELKEY(channelName);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }

        if (_channelsList[channelIndex].getL() != 0 && 
            _channelsList[channelIndex].getChatters().size() >= _channelsList[channelIndex].getL()) 
        {
            std::string err = ERR_CHANNELISFULL(channelName);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
    }

    // Ajouter l'utilisateur au channel
    _channelsList[channelIndex].getChatters().push_back(nickname);
    user.setChannel(_channelsList[channelIndex]);

    // Message de join
    std::string joinMessage = ":" + nickname + "!" + username + "@" + hostname + " JOIN " + channelName + "\r\n";
    const std::vector<std::string>& chatters = _channelsList[channelIndex].getChatters();
    for (size_t i = 0; i < chatters.size(); ++i) {
        User* channelUser = getUserByNickname(chatters[i]);
        if (channelUser) {
            send(channelUser->getSocket(), joinMessage.c_str(), joinMessage.size(), 0);
        }
    }

    // Topic
    std::string rpl;
    if (!_channelsList[channelIndex].getTopic().empty())
        rpl = RPL_TOPIC(channelName, _channelsList[channelIndex].getTopic());
    else
        rpl = RPL_NOTOPIC(channelName);
    send(user.getSocket(), rpl.c_str(), rpl.size(), 0);

    // Liste des utilisateurs
    std::string userList = ":" + _name + " 353 " + nickname + " = " + channelName + " :";
    for (size_t i = 0; i < chatters.size(); ++i) {
        userList += chatters[i] + " ";
    }
    userList += "\r\n";
    send(user.getSocket(), userList.c_str(), userList.size(), 0);

    std::string endOfList = ":" + _name + " 366 " + nickname + " " + channelName + " :End of /NAMES list.\r\n";
    send(user.getSocket(), endOfList.c_str(), endOfList.size(), 0);
}