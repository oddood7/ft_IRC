/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:08:48 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/13 12:34:29 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

void	Server::nick(User &user)
{
	if (user.getBuf().size() == 1) {
		std::cerr << RED << ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]) << RESET << std::endl;
		return ;
	}
	std::string nickname;

	for (size_t i = 1; i < user.getBuf().size(); i++){
		nickname += user.getBuf()[i];
		if (i < user.getBuf().size() - 1)
			nickname += " ";
	}
	
	if (user.getBuf().size() > 2){
		std::cerr << RED << ERR_ERRONEUSNICKNAME(_name, nickname) << RESET << std::endl;
	}
	else if (!user.getPass().empty())
	{
		for (size_t i = 0; i < user.getBuf()[1].size(); i++)
		{
			if (!isalnum(user.getBuf()[1][i]))
			{
				std::cerr << RED << ERR_ERRONEUSNICKNAME(_name, nickname) << RESET << std::endl;
				user.getBuf().clear();
				return ;
			}
		}
		for (std::map<int, User>::iterator ite = usersManage.begin(); ite != usersManage.end(); ite++)
		{
			if (nickname.size() > 9)
				nickname = nickname.substr(0, 9);
			if (nickname == ite->second.getNickName())
			{
				std::cerr << RED << ERR_NICKNAMEINUSE(_name, nickname) << RESET << std::endl;
				user.getBuf().clear();
				return ;
			}
		}	
		std::string str = "NICK set to " + nickname + "\r\n";
		user.setNickName(nickname);
		send(user.getSocket(), str.c_str(), str.size(), 0);
	}
	else {
		std::cerr << RED << ERR_NOTREGISTERED(_name) << RESET << std::endl;
		std::cerr << RED << "Enter PASS, NICK, USER in this order" << RESET <<std::endl;
	}
}

void	Server::nickIrssi(User &user, int i)
{
	for (std::map<int, User>::iterator ite = usersManage.begin(); ite != usersManage.end(); ++ite)
	{
		if (user.getBuf().size() > (size_t)i){
			if (ite->second.getNickName() == user.getBuf()[i + 1]){
				std::stringstream ss;
				ss << _activeUsers;
				std::string str = ss.str();
				user.setNickName(user.getBuf()[i + 1] + str);
				user.setRpl("NICK command accepted with modification.\n");
				return;
			}
		}
	}
	if (user.getBuf().size() > (size_t)i)
		user.setNickName(user.getBuf()[i + 1]);
	else
		user.setNickName("default");	
}
