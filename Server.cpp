/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:39:34 by lde-mais          #+#    #+#             */
/*   Updated: 2024/04/20 12:45:43 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "Server.hpp"

Server::Server() {
	return ;
}

Server::~Server() {
	return ;
}

Server::Server(int port, std::string pswd) : _port(port), _name("IRC"), _password(pswd), _stop(false)
{
	//on construit un objet server avec un nom un password un port et un bool
	//pour pouvoir stop le serveur si besoin
	//on gererait ensuite l'ajout de client et la creation d'une adresse?

	for (size_t i = 0; i < _password.size(); i++)
	{
		if (_password[i] == 32)
			std::cerr << "Invalid Password" << std::endl;
	}
	//on check ici si le mot de passe a les bon parametres genre un
	//espace au milieu
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw ("Server socket error");
	}
	
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_socket, (struct sockaddr *)&_address, _size) == -1)
	{
		close(_socket);
		throw("Bind Error"); //faudra gerer les try and catch mieux
	}
	if (listen(_socket, 5) == -1) //5 = clients max donc faudra voir si on veut DEFINE
	{
		close(_socket);
		throw("Listen error");
	}
	std::cout << _name << " server started on port : " << _port << std::endl;
}

// void  Server::run()
// {
//   while (_stop == false)
//   {
//     //etc
//   }
// }