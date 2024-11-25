/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:33:03 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 14:18:40 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <sstream>
# include <vector>
# include <netinet/in.h>
# include <string>
# include <stdexcept>
# include "Channel.hpp"

class User
{
	private:
		
		int _socket;
		int	_id;
		

		sockaddr_in _addr;

		std::string _hostName;
		std::string _realName;
		std::string _nickName;
		std::string _userName;
		std::string _serverName;
		std::string _host;
		std::string _pass;
		std::string _rpl;
		std::string _partialCommand;

		std::vector<std::string>	_buf;

		std::string 	_activeChannel;

		bool _invisible;
		bool _irssi;
		bool _verified;
		bool _deco;

	public:
	
		User();
		User(const User& cpy);
		~User();
		User(int sock, sockaddr_in addr);

		User & operator=(User const & cpy);

		void 		appendPartialCommand(const std::string& partial);
		std::string getAndClearPartialCommand();
		int			getSocket()  const {return _socket;};
		int			getId()  const {return _id;};
		std::string	getNickName() const {return _nickName;};
		std::string	getHostName() const {return _hostName;};
		std::string	getUserName() const {return _userName;};
		std::string	getPass() const {return _pass;};
		std::string getHost() const {return _host;};
		std::string getRpl() const {return _rpl;};
		bool		getMode() const {return _invisible;};
		bool		getVerif() const {return _verified;};
		bool		getIrssi() const {return _irssi;};
		bool		getDeco() const {return _deco;};
		std::string		&getChannel() {return _activeChannel;};
		std::vector<std::string>	&getBuf() {return _buf;};
		
		void	setId(int i) {_id = i;};
		void	setNickName(const std::string &nick) {_nickName = nick;};
		void	setRealName(const std::string &real) {_realName = real;};
		void 	setHostName(const std::string &hostname) {_hostName = hostname;};
		void 	setServerName(const std::string &servername) {_serverName = servername;};
		void	setPass(const std::string &pass) {_pass = pass;};
		void	setRpl(const std::string &rpl) {_rpl = rpl;};
		void	setMode(bool i) {_invisible = i;};
		void	setVerif();
		void	setUserName(const std::string &username) {_userName = username;};
		void	setDeco() {_deco = true;};
		void	setIrssi() {_irssi = true;};
		void	setChannel(Channel &channel);
		void	setBuf(const std::string buffer);


};

#endif