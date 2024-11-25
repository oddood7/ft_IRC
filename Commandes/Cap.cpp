/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:39:45 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/23 15:08:42 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cap(User &user)
{
    if (user.getBuf().size() < 2) {
        std::cerr << RED << ERR_NEEDMOREPARAMS(_name, "CAP") << RESET << std::endl;
        return;
    }

    std::string subcommand = user.getBuf()[1];
    if (subcommand == "LS") {
        std::string capResponse = ":" + _name + " CAP * LS :\r\n";
        send(user.getSocket(), capResponse.c_str(), capResponse.size(), 0);
    } else if (subcommand == "END") {
        if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getPass().empty()) {
            std::string userIdent = user.getUserName() + "!" + user.getNickName() + "@localhost";
            user.setRpl(RPL_WELCOME(userIdent, user.getNickName()));
            send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
            std::cout << GREEN << "User " << user.getId() << " connected as " << user.getUserName() << RESET << std::endl;
            user.setIrssi();
            user.setVerif();
        } else {
            std::cerr << RED << "Failed to fully authenticate user." << RESET << std::endl;
        }
    }
}