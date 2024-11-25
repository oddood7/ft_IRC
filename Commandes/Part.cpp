/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:11:08 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 14:18:47 by lde-mais         ###   ########.fr       */
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

// void Server::part(User &user)
// {
//     if (user.getBuf().size() < 2) {
//         std::string err = ERR_NEEDMOREPARAMS(user.getHost(), "PART");
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::string channelName = user.getBuf()[1];
//     std::string reason = (user.getBuf().size() > 2) ? user.getBuf()[2] : "Leaving";

//     Channel* channel = NULL;
//     for (size_t i = 0; i < _channelsList.size(); ++i) {
//         if (_channelsList[i].getName() == channelName) {
//             channel = &_channelsList[i];
//             break;
//         }
//     }

//     if (!channel) {
//         std::string err = ERR_NOSUCHCHANNEL(channelName);
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::vector<std::string>& chatters = channel->getChatters();
//     std::vector<std::string>::iterator it = std::find(chatters.begin(), chatters.end(), user.getNickName());

//     if (it == chatters.end()) {
//         std::string err = ERR_NOTONCHANNEL(channelName);
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     chatters.erase(it);

//     std::string partMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " PART " + channelName + " :" + reason + "\r\n";
    
//     for (std::vector<std::string>::iterator it = chatters.begin(); it != chatters.end(); ++it) {
//         User* chatUser = getUserByNickname(*it);
//         if (chatUser) {
//             send(chatUser->getSocket(), partMessage.c_str(), partMessage.size(), 0);
//         }
//     }
//     send(user.getSocket(), partMessage.c_str(), partMessage.size(), 0);

//     user.setChannel(Channel());

//     if (chatters.empty()) {
//         for (std::vector<Channel>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
//             if (it->getName() == channelName) {
//                 _channelsList.erase(it);
//                 break;
//             }
//         }
//     }
// }

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
    size_t channelIndex = 0;
    for (size_t i = 0; i < _channelsList.size(); ++i) {
        if (_channelsList[i].getName() == channelName) {
            channel = &_channelsList[i];
            channelIndex = i;
            break;
        }
    }

    if (!channel) {
        std::string err = ERR_NOSUCHCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    std::vector<std::string>& chatters = channel->getChatters();
    bool found = false;
    size_t userIndex = 0;
    
    for (size_t i = 0; i < chatters.size(); ++i) {
        if (chatters[i] == user.getNickName()) {
            found = true;
            userIndex = i;
            break;
        }
    }

    if (!found) {
        std::string err = ERR_NOTONCHANNEL(channelName);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }

    // Supprimer l'utilisateur de la liste des chatters
    for (size_t i = userIndex; i < chatters.size() - 1; ++i) {
        chatters[i] = chatters[i + 1];
    }
    chatters.pop_back();

    std::string partMessage = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " PART " + channelName + " :" + reason + "\r\n";
    
    // Notifier les autres utilisateurs
    for (size_t i = 0; i < chatters.size(); ++i) {
        User* chatUser = getUserByNickname(chatters[i]);
        if (chatUser) {
            send(chatUser->getSocket(), partMessage.c_str(), partMessage.size(), 0);
        }
    }
    send(user.getSocket(), partMessage.c_str(), partMessage.size(), 0);

    // Réinitialiser le channel de l'utilisateur
    Channel emptyChannel;
    user.setChannel(emptyChannel);

    // Si le channel est vide, le supprimer
    if (chatters.empty()) {
        for (size_t i = channelIndex; i < _channelsList.size() - 1; ++i) {
            _channelsList[i] = _channelsList[i + 1];
        }
        _channelsList.pop_back();
    }
}






