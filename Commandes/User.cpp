/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:15:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 13:55:20 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::user(User &user)
{
	 if (user.getPass().empty() || user.getPass().compare(_password)) {
        std::string err = ":" + _name + " 464 * :Password required first\r\n";
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
	
    if (user.getBuf().size() < 5) {
        std::string err = ERR_NEEDMOREPARAMS(_name, "USER");
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    if (!user.getUserName().empty()) {
        std::string err = ERR_ALREADYREGISTRED(_name);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    std::string username = user.getBuf()[1];
    std::string hostname = user.getBuf()[2];
    std::string realname = user.getBuf()[4];
    
    for (size_t i = 5; i < user.getBuf().size(); ++i) {
        realname += " " + user.getBuf()[i];
    }

    std::string baseUsername = username;
    int suffix = 1;
    bool isUnique = false;

    while (!isUnique) {
        isUnique = true;
        for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it) {
            if (it->second.getUserName() == username) {
                isUnique = false;
                std::stringstream ss;
                ss << baseUsername << suffix;
                username = ss.str();
                suffix++;
                break;
            }
        }
    }

    user.setUserName(username);
    user.setHostName(hostname);
    user.setRealName(realname);

    std::cout << GREEN << "User registered as " << username << RESET << std::endl;

    if (!user.getNickName().empty() && !user.getPass().empty()) {
        std::string userIdent = user_id(user.getUserName(), user.getNickName());
        std::string rpl = RPL_WELCOME(userIdent, user.getNickName());
        send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
        user.setVerif();
    }
}