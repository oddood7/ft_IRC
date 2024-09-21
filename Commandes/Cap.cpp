/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:39:45 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/21 11:26:34 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cap(User &user)
{
	if (!user.getIrssi())
    {	
        bool passw = false;
        bool nick = false;
        bool userCmd = false;

        for (size_t i = 0; i < user.getBuf().size(); i++)
        {
            std::string command = user.getBuf()[i];

            if (command == "PASS")
            {
                passw = passIrssi(user, i);
            }
            else if (command == "NICK")
            {
                nickIrssi(user, i);
                nick = true;
            }
            else if (command == "USER")
            {
                userIrssi(user, i);
                userCmd = true;
            }
        }
		if (passw && nick && userCmd)
		{
    		std::string userIdent = user.getUserName() + "!" + user.getNickName() + "@localhost";
    		user.setRpl(RPL_WELCOME(userIdent, user.getNickName()));
    		send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
    		std::cout << GREEN << "User " << user.getId() << " connected as " << user.getUserName() << RESET << std::endl;
    		user.setIrssi();
    		user.setVerif();
		}
        else
        {
            std::cerr << RED << "Failed to fully authenticate user." << RESET << std::endl;
        }
    }
    else
    {
        std::cerr << RED << ERR_UNKNOWNCOMMAND(user.getBuf()[0]) << RESET << std::endl;
    }
}

// void Server::cap(User &user)
// {
//     if (user.getBuf().size() < 2 || user.getBuf()[0] != "CAP") {
//         std::cerr << RED << "Invalid CAP command" << RESET << std::endl;
//         return;
//     }

//     if (user.getBuf()[1] == "LS") {
//         // Répondre à CAP LS
//         std::string capResponse = "CAP * LS :\r\n";
//         send(user.getSocket(), capResponse.c_str(), capResponse.size(), 0);
//     } else if (user.getBuf()[1] == "END") {
//         // Fin de la négociation CAP, vérifier si l'utilisateur est authentifié
//         if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getPass().empty()) {
//             std::string userIdent = user.getUserName() + "!" + user.getNickName() + "@localhost";
//             user.setRpl(RPL_WELCOME(userIdent, user.getNickName()));
//             send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
//             std::cout << GREEN << "User " << user.getId() << " connected as " << user.getUserName() << RESET << std::endl;
//             user.setIrssi();
//             user.setVerif();
//         } else {
//             std::cerr << RED << "Failed to fully authenticate user." << RESET << std::endl;
//         }
//     } else {
//         // Commande CAP non reconnue
//         std::cerr << RED << ERR_UNKNOWNCOMMAND(user.getBuf()[1]) << RESET << std::endl;
//     }
// }
// void Server::cap(User &user)
// {
//     const std::vector<std::string>& buf = user.getBuf();
    
//     if (buf.size() < 2) {
//         std::string err = ERR_NEEDMOREPARAMS(_name, "CAP");
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//         return;
//     }

//     std::string subcommand = buf[1];

//     if (subcommand == "LS") {
//         std::string capResponse = ":" + _name + " CAP * LS :\r\n";
//         send(user.getSocket(), capResponse.c_str(), capResponse.size(), 0);
//     } else if (subcommand == "END") {
//         if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getPass().empty()) {
//             std::string userIdent = user_id(user.getUserName(), user.getNickName());
//             std::string rpl = RPL_WELCOME(userIdent, user.getNickName());
//             send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
//             std::cout << GREEN << "User " << user.getId() << " connected as " << user.getUserName() << RESET << std::endl;
//             user.setIrssi();
//             user.setVerif();
//         } else {
//             std::string err = ERR_NOTREGISTERED(_name);
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//         }
//     } else {
//         std::string unknownCommand = "CAP " + subcommand;
//         std::string err = ERR_UNKNOWNCOMMAND(unknownCommand.c_str());
//         send(user.getSocket(), err.c_str(), err.size(), 0);
//     }
// }
