/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 18:22:59 by lde-mais          #+#    #+#             */
/*   Updated: 2024/08/13 15:19:23 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void	Server::quit(User &user)
{
	std::string str = RPL_QUIT(user_id(user.getUserName(), user.getNickName()), "Leave");
	send(user.getSocket(), str.c_str(), str.size(), 0);
	user.setDeco();
	deleteFromChannel(user);
	deleteUser(user);
}
