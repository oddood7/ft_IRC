/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:20:53 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/04 15:23:32 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "Server.hpp"

# define RESET   "\033[0m"
# define RED     "\033[1;31m"
# define GREEN   "\033[1;32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"

# define RPL_WELCOME(user_id, nick) ":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + user_id + "\r\n"
# define RPL_QUIT(user_id, reason) user_id + " QUIT :Quit: " + reason + "\r\n"
# define RPL_UMODEIS(client, mode) ":localhost 221 " + client + " " + mode + "\r\n"
# define RPL_NOTOPIC(channel) ":localhost 331 " + channel + " :No topic set for " + channel + "\r\n"
# define RPL_TOPIC(channel, topic)":localhost 332 " + channel + " " + channel + " " + topic + "\r\n"
# define RPL_INVITING(channel, nickname) channel + " " + nickname + "\r\n"
# define RPL_KICK(channel, operator, nickname, reason) ":" + operator + " KICK " + channel + " " + nickname + " " + reason + "\r\n"
# define RPL_PRIV(nickname, target, message) ":" + nickname + " PRIVMSG "  + target + " " + message + "\r\n"
# define RPL_NOTICE(nickname, target, message) ":" + nickname + " NOTICE " + target + " " + message + "\r\n"
# define MODE_USERMSG(client, mode) ":" + client + " MODE " + client + " :" + mode + "\r\n"
# define MODE_CHANNELMSG(channel, mode) ":" + channel + " MODE " + channel + " :" + mode + "\r\n"

#endif