/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:39:45 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/15 13:36:27 by lde-mais         ###   ########.fr       */
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

            std::cerr << "Sending response: " << user.getRpl() << std::endl;
            send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
        }

        if (passw && nick && userCmd)
        {
            user.setRpl(RPL_WELCOME(user_id(user.getUserName(), user.getNickName()), user.getNickName()));
            send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
            std::cout << GREEN << "User " << user.getId() << " connected" << RESET << std::endl;
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

