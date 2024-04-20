/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:30:33 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/20 11:51:57 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage :" << av[0] << " port + password" << std::endl;
		return 1;
		//peux faire un try and catch au lieu mais voila t'as compris
	}

	Server server(std::atoi(av[1]), av[2]);
	// int port = atoi(av[1]);
	// std::string pswd = av[2];

	//on prend en args les parametres pour ce login

	return 0;
}