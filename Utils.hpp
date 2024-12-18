/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:38:16 by lde-mais          #+#    #+#             */
/*   Updated: 2024/09/23 15:21:47 by lde-mais         ###   ########.fr       */
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
# define CYAN    "\033[1;96m"
# define WHITE_BOLD "\033[1;97m"

# define user_id(user, nick) ":" + user + "!" + nick + "@localhost"

#define ERR_UNKNOWNCOMMAND(command) (std::string(":localhost 421 ") + command + " :Unknown command\r\n")
# define ERR_NOSUCHNICK(nick) ":localhost 401 " + nick + " :No such nick/channel\r\n"
# define ERR_NOSUCHCHANNEL(channel) ":localhost 403 " + channel + " :No such channel\r\n"
# define ERR_NONICKNAMEGIVEN(host) host + " 431 :No nick name given\r\n"
# define ERR_ERRONEUSNICKNAME(host, nickname) host + " 432 " + nickname + " :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(host, nickname) host + " 433 " + nickname + " :Nickname is already in use\r\n"
# define ERR_USERNOTINCHANNEL(nickname, channel) ":localhost 441 " + nickname + " " + channel + "\r\n"
# define ERR_NOTONCHANNEL(channel) ":localhost 442 " + channel + " :You're not on that channel\r\n"
# define ERR_USERONCHANNEL(nickname, channel) ":localhost 443 " + nickname + " " + channel + " :is already on channel\r\n"
# define ERR_NOTREGISTERED(host) host + " 451 :You have not registered\r\n"
# define ERR_NEEDMOREPARAMS(host, command) host + " 461 " + command + " :Not enough parameters\r\n"
# define ERR_ALREADYREGISTRED(host) host + " 462 :You may not reregister\r\n"
# define ERR_PASSWDMISMATCH(host) host + " 464 :Password incorrect\r\n"
# define ERR_CHANNELISFULL(channel) ":localhost 471 " + channel + " :" + channel + "\r\n"
# define ERR_UNKNOWNMODE(host, ch) host + " 472 " + ch + " :is unknown mode char to me\r\n"
# define ERR_INVITEONLYCHAN(channel) ":localhost 473 " + channel + " :" + channel + "\r\n"
# define ERR_BADCHANNELKEY(channel) ":localhost 475 " + channel + " :" + channel + "\r\n"
# define ERR_CHANOPRIVSNEEDED(channel) ":localhost 482 " + channel + " :You're not channel operator\r\n"
# define ERR_USERSDONTMATCH(host) host + " 502 :Cant change mode for other users\r\n"
# define ERR_ERRONEUSUSER(host, user) host + " " + user + " :Erroneus user\r\n"
# define ERR_BADPASS(host) host + " :Bad Password\r\n"
# define ERR_BADMODEL(host) host + " :Bad L mode size\r\n"

# define RPL_PART(user_id, channel, reason) (user_id + " PART " + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")
# define RPL_WELCOME(user_id, nick) ":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + user_id + "\r\n"
# define RPL_QUIT(user_id, reason) user_id + " QUIT :Quit: " + reason + "\r\n"
# define RPL_UMODEIS(client, mode) ":localhost 221 " + client + " " + mode + "\r\n"
# define RPL_NOTOPIC(channel) ":localhost 331 " + channel + " :No topic set for " + channel + "\r\n"
# define RPL_TOPIC(channel, topic)":localhost 332 " + channel + " " + channel + " " + topic + "\r\n"
# define RPL_TOPICWHOTIME(channel, nick, setat) ("333 " + channel + " " + nick + " " + setat + "\r\n")
# define RPL_INVITING(channel, nickname) channel + " " + nickname + "\r\n"
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")
# define RPL_KICK(channel, operator, nickname, reason) ":" + operator + " KICK " + channel + " " + nickname + " " + reason + "\r\n"
# define RPL_PRIV(nickname, target, message) ":" + nickname + "!~" + nickname + "@localhost PRIVMSG " + target + " :" + message + "\r\n"
# define RPL_NOTICE(nickname, target, message) ":" + nickname + " NOTICE " + target + " " + message + "\r\n"
# define MODE_USERMSG(client, mode) ":" + client + " MODE " + client + " :" + mode + "\r\n"
# define MODE_CHANNELMSG(channel, mode) ":" + channel + " MODE " + channel + " :" + mode + "\r\n"

#endif