/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:17:46 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/20 14:05:44 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::topic(User &user)
{
    if (user.getBuf().size() < 2) {
        std::string ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
        return;
    }

    std::string channelName = user.getBuf()[1];
    if (isInChannel(user, channelName))
        return;

    Channel &channel = getUserChannel(channelName);

    if (user.getBuf().size() == 2)
    {
        std::string rpl;
        if (channel.getTopic().empty())
            rpl = RPL_NOTOPIC(channel.getName());
        else {
            rpl = RPL_TOPIC(channel.getName(), channel.getTopic());
            rpl += RPL_TOPICWHOTIME(channel.getName(), "unknown", "0");
        }
        send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
    }
    else if (user.getBuf().size() > 2)
    {
        if (channel.getT() && is_op(user, user.getNickName()) == -1) {
            std::string err = ERR_CHANOPRIVSNEEDED(channel.getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }

        std::string topic;
        for (size_t i = 2; i < user.getBuf().size(); i++) {
            topic += user.getBuf()[i];
            if (i < user.getBuf().size() - 1)
                topic += " ";
        }

        channel.setTopic(topic);
        std::string topicMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHost() + " TOPIC " + channel.getName() + " :" + topic + "\r\n";
        std::vector<std::string> &chatters = channel.getChatters();
        for (std::vector<std::string>::iterator it = chatters.begin(); it != chatters.end(); ++it) {
            for (std::map<int, User>::iterator uit = usersManage.begin(); uit != usersManage.end(); ++uit) {
                if (uit->second.getNickName() == *it) {
                    send(uit->second.getSocket(), topicMsg.c_str(), topicMsg.size(), 0);
                    std::string rpl_topic = RPL_TOPIC(channel.getName(), topic);
                    send(uit->second.getSocket(), rpl_topic.c_str(), rpl_topic.size(), 0);    
                    break;
                }
            }
        }
    }
}