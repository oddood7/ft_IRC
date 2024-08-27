/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:15:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/27 14:35:22 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

// void	Server::user(User &user)
// {		
// 	std::string str;

// 	for (size_t i = 1; i < user.getBuf().size(); i++){
// 		str += user.getBuf()[i];
// 		if (i < user.getBuf().size() - 1)
// 			str += " ";
// 	}

// 	if (user.getNickName().empty() || user.getPass().empty()){
// 		std::cerr << RED << ERR_NOTREGISTERED(_name) << RESET << std::endl;
// 		std::cerr << RED << "Enter PASS, NICK, USER in this order" << RESET << std::endl;
// 		return;
// 	}
// 	else if (user.getBuf().size() == 1){
// 		std::cerr << RED << ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]) << RESET << std::endl;
// 		return;
// 	}
// 	else if (user.getBuf().size() > 2){
// 		std::cerr << RED << ERR_ERRONEUSUSER(_name, str) << RESET << std::endl;
// 		return;
// 	}
// 	else if (user.getUserName().empty() == false){
// 		std::cerr << RED << ERR_ALREADYREGISTRED(_name) << RESET << std::endl;
// 		return;
// 	}
// 	else
// 	{
// 		for (size_t i = 0; i < user.getBuf()[1].size(); i++)
// 		{
// 			if (!isalnum(user.getBuf()[1][i]))
// 			{
// 				std::cerr << RED << ERR_ERRONEUSUSER(_name, str) << RESET << std::endl;
// 				return ;
// 			}
// 		}
// 		user.setUserName(user.getBuf()[1]);
// 		std::cout << GREEN << "User" << user.getId() << " connected" << RESET << std::endl;
// 		user.setRpl(RPL_WELCOME(user_id(str, user.getNickName()), user.getNickName()));
// 		send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
// 		return ;
// 	}
// }

// void	Server::userIrssi(User &user, int i)
// {
// 	std::cout << "Debug: Entering userIrssi" << std::endl;
//     std::cout << "Debug: User buffer content: ";
//     for (size_t j = 0; j < user.getBuf().size(); ++j) {
//         std::cout << "'" << user.getBuf()[j] << "' ";
//     }
//     std::cout << std::endl;
// 	std::string str;
// 	for (size_t j = i + 1; user.getBuf()[j] != "localhost" && j < user.getBuf().size(); j++){
// 		str += user.getBuf()[j];
// 		if (user.getBuf().size() > j){
// 			if (user.getBuf()[j + 1] != "localhost")
// 				str += " ";
// 		}
// 	}
// 	user.setUserName(str);
// 	//user.setRpl("USER command accepted.\n");
// }

void Server::user(User &user)
{
    if (user.getNickName().empty() || user.getPass().empty()) {
        std::cerr << RED << ERR_NOTREGISTERED(_name) << RESET << std::endl;
        std::cerr << RED << "Enter PASS, NICK, USER in this order" << RESET << std::endl;
        return;
    }

    std::string username;
    if (user.getBuf().size() >= 5) {  // Format IRSSI
        username = user.getBuf()[1];
    } else if (user.getBuf().size() == 2) {  // Format nc
        username = user.getBuf()[1];
    } else {
        std::cerr << RED << ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]) << RESET << std::endl;
        return;
    }

    // Vérifier la validité du nom d'utilisateur
    for (size_t i = 0; i < username.size(); i++) {
        if (!isalnum(username[i])) {
            std::cerr << RED << ERR_ERRONEUSUSER(_name, username) << RESET << std::endl;
            return;
        }
    }

    user.setUserName(username);

    std::cout << GREEN << "User" << user.getId() << " connected as " << username << RESET << std::endl;
    std::string userIdent = user_id(username, user.getNickName());
    user.setRpl(RPL_WELCOME(userIdent, user.getNickName()));
    send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
    
    user.setVerif();  // Marquer l'utilisateur comme vérifié
}