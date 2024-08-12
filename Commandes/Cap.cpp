/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:39:45 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/24 11:51:15 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

// void	Server::cap(User &user)
// {
// 	if (user.getIrssi() == false)
// 	{
// 		bool passw = false;
// 		for (size_t i = 0; i < user.getBuf().size(); i++)
// 		{
// 			if (user.getBuf()[i] == "PASS")
// 			{
// 				std::cerr << "Received command: " << user.getBuf()[i] << std::endl;
// 				passw = passIrssi(user, i);
// 				std::cerr << "Sending response: " << user.getRpl() << std::endl;
// 			}
// 			if (user.getBuf()[i] == "NICK")
// 			{
// 				std::cerr << "Received command: " << user.getBuf()[i] << std::endl;
// 				nickIrssi(user, i);
// 				std::cerr << "Sending response: " << user.getRpl() << std::endl;
// 			}
// 			if (user.getBuf()[i] == "USER")
// 			{
// 				std::cerr << "Received command: " << user.getBuf()[i] << std::endl;
// 				userIrssi(user, i);
// 				std::cerr << "Sending response: " << user.getRpl() << std::endl;
// 			}
// 		}
// 		if (passw)
// 		{
// 			user.setRpl(RPL_WELCOME(user_id(user.getUserName(), user.getNickName()), user.getNickName()));
// 			send(user.getSocket(), user.getRpl().c_str(), user.getRpl().size(), 0);
// 			std::cout << GREEN << "User " << user.getId() << " connected" << RESET << std::endl;
// 			user.setIrssi();
// 			user.setVerif();
// 		}
// 		else
// 		{
// 			std::cerr << RED << "Failed to fully authenticate user." << RESET << std::endl;
// 		}
// 	}
// 	else
// 		std::cerr << RED << ERR_UNKNOWNCOMMAND(user.getBuf()[0]) << RESET << std::endl;		
// }

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
            std::cerr << "Received command: " << command << std::endl;

            if (command == "PASS")
            {
                passw = passIrssi(user, i);
                std::cerr << "Processed PASS command. Result: " << (passw ? "Success" : "Failure") << std::endl;
            }
            else if (command == "NICK")
            {
                nickIrssi(user, i);
                nick = true;
                std::cerr << "Processed NICK command." << std::endl;
            }
            else if (command == "USER")
            {
                userIrssi(user, i);
                userCmd = true;
                std::cerr << "Processed USER command." << std::endl;
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