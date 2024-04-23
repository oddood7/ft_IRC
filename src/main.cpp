/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diavolo <diavolo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:30:33 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/23 15:07:16 by diavolo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		try {
			Server server(std::atoi(av[1]));
			server.run();
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
		}			
	}
	else
		std::cerr << "Need 3 Args" << std::endl;
	// int port = atoi(av[1]);
	// std::string pswd = av[2];

	//on prend en args les parametres pour ce login

	return 0;
}