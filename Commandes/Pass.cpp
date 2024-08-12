/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:54:22 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/24 11:20:30 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../Utils.hpp"

void Server::pass(User &user)
{
    std::cerr << "Buffer size: " << user.getBuf().size() << std::endl;
    if (user.getBuf().size() == 1) {
        std::cerr << RED << ERR_NEEDMOREPARAMS(_name, user.getBuf()[0]) << RESET << std::endl;
        return;
    }
    else if (user.getBuf().size() > 2) {
        std::cerr << RED << ERR_PASSWDMISMATCH(_name) << RESET << std::endl;
        return ;
    }
    else if (user.getPass().empty())
    {
        if (user.getBuf()[1].compare(_password)) {
            std::cerr << RED << ERR_PASSWDMISMATCH(_name) << RESET << std::endl;
            user.setDeco();
            deleteUser(user);
            return ;
        }
        else {
            std::cerr << "Password accepted." << std::endl;
            user.setPass(user.getBuf()[1]);
            return ;
        }
    }
    else
        std::cerr << RED << ERR_ALREADYREGISTRED(_name) << RESET << std::endl;
}


bool Server::passIrssi(User &user, int i)
{
    if (user.getBuf().size() >= ((size_t)i + 2))
    {
        if (user.getBuf()[i + 2] != "NICK")
        {
            std::cerr << RED << ERR_PASSWDMISMATCH(_name) << RESET << std::endl;
            return false;
        }
        else if (user.getBuf()[i + 1] != _password)
        {
            std::cerr << RED << ERR_PASSWDMISMATCH(_name) << RESET << std::endl;
            return false;
        }
        else
        {
            std::cerr << "Password accepted by passIrssi." << std::endl;
            user.setPass(user.getBuf()[i + 1]);
        }
    }
    return true;
}
