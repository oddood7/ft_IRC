/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:37:31 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/23 15:19:21 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 3)
	{
        try {
            Server server(std::atoi(av[1]), av[2]);
			signal(SIGINT, Server::SignalHandler);
  			signal(SIGQUIT, Server::SignalHandler);
            server.run();
			signal(SIGINT, Server::SignalHandler); 
  			signal(SIGQUIT, Server::SignalHandler); 
        }
        catch (std::exception &e) {
        }
    }
    else
        std::cerr << RED << "Wrong number of arguments" << RESET << std::endl;
}