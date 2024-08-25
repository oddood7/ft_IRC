/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:15:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/25 17:52:56 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void	Server::user(User &user)
{		
	std::string str;

	for (size_t i = 1; i < user.getBuf().size(); i++){
		str += user.getBuf()[i];
		if (i < user.getBuf().size() - 1)
			str += " ";
	}

	if (user.getNickName().empty() || user.getPass().empty()){
		std::cerr << RED << ERR_NOTREGISTERED(_name) << RESET << std::endl;
		std::cerr << RED << "Enter PASS, NICK, USER in this order" << RESET << std::endl;
		return;
	}
	else if (user.getBuf().size() == 1){
		std::cerr << RED << ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]) << RESET << std::endl;
		return;
	}
	else if (user.getBuf().size() > 2){
		std::cerr << RED << ERR_ERRONEUSUSER(_name, str) << RESET << std::endl;
		return;
	}
	else if (user.getUserName().empty() == false){
		std::cerr << RED << ERR_ALREADYREGISTRED(_name) << RESET << std::endl;
		return;
	}
	else
	{
		for (size_t i = 0; i < user.getBuf()[1].size(); i++)
		{
			if (!isalnum(user.getBuf()[1][i]))
			{
				std::cerr << RED << ERR_ERRONEUSUSER(_name, str) << RESET << std::endl;
				return ;
			}
		}
		user.setUserName(user.getBuf()[1]);
		std::cout << GREEN << "User" << user.getId() << " connected" << RESET << std::endl;
		user.setRpl(RPL_WELCOME(user_id(str, user.getNickName()), user.getNickName()));
		send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
		return ;
	}
}

void	Server::userIrssi(User &user, int i)
{
	std::string str;
	for (size_t j = i + 1; user.getBuf()[j] != "localhost" && j < user.getBuf().size(); j++){
		str += user.getBuf()[j];
		if (user.getBuf().size() > j){
			if (user.getBuf()[j + 1] != "localhost")
				str += " ";
		}
	}
	user.setUserName(str);
	//user.setRpl("USER command accepted.\n");
}

// void Server::userIrssi(User &user, int i)
// {    
//     // Vérifier que les paramètres sont suffisants
//     if (i + 3 >= static_cast<int>(user.getBuf().size())) {
//         std::cerr << RED << "Error: USER command is incomplete" << RESET << std::endl;
//         return;
//     }

//     // Extraire le nom d'utilisateur depuis les paramètres
//     std::string username = user.getBuf()[i + 1];

//     // Vérifier si le nom d'utilisateur existe déjà dans les utilisateurs connectés
//     bool usernameExists = false;
//     for (std::map<int, User>::iterator it = usersManage.begin(); it != usersManage.end(); ++it) {
//         if (it->second.getUserName() == username) {
//             usernameExists = true;
//             break;
//         }
//     }

//     // Si le nom d'utilisateur existe déjà, utiliser le surnom
//     if (usernameExists) {
//         username = user.getNickName();
//     }

//     // Définir le nom d'utilisateur de l'utilisateur
//     user.setUserName(username);

//     // Conserver les autres paramètres de la commande USER
//     // Par exemple, vous pouvez traiter d'autres paramètres si nécessaire ici.
// }
