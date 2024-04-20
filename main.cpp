/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:30:33 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/20 12:52:56 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int ac, char **av)
{
	if (ac == 3)
	{
		try {
			Server server(std::atoi(av[1]), av[2]);
		}
		catch (std::exception &e) {}			
	}
	else
		std::cerr << "Need 3 Args" << std::endl;
	// int port = atoi(av[1]);
	// std::string pswd = av[2];

	//on prend en args les parametres pour ce login

	return 0;
}