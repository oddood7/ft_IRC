/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:31:44 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 14:18:37 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Utils.hpp"

User::User() : _socket(0), _id(0), _invisible(false), _irssi(false), _verified(false), _deco(false){}

User::~User() {}

User::User(int sock, sockaddr_in addr) :_socket(sock), _id(0), \
	_addr(addr), _host("localhost"), \
	_invisible(false), _irssi(false), _verified(false), _deco(false) {

}

User::User(const User& cpy) : _socket(cpy._socket), _id(cpy._id), \
	_addr(cpy._addr),  _nickName(cpy._nickName), _userName(cpy._userName), \
	_host(cpy._host), _pass(cpy._pass),_rpl(cpy._rpl), _buf(cpy._buf), \
	_invisible(cpy._invisible), _irssi(cpy._irssi), _verified(cpy._verified), _deco(cpy._deco) {

}

User & User::operator=(User const & cpy)
{
	if (this != &cpy){
		this->_socket = cpy._socket;
		this->_id = cpy._id;
		this->_addr = cpy._addr;
		this->_nickName = cpy._nickName;
		this->_userName = cpy._userName;
		this->_host = cpy._host;
		this->_pass = cpy._pass;
		this->_rpl = cpy._rpl;
		this->_buf = cpy._buf;
		this->_invisible = cpy._invisible;
		this->_irssi = cpy._irssi;
		this->_verified = cpy._verified;
		this->_deco = cpy._deco;
	}
	return *this;
}


void User::setBuf(const std::string buffer)
{
	_buf.clear();
    std::istringstream iss(buffer);
    std::string word;
    while (iss >> word)
    {
        _buf.push_back(word);
    }
}// void	Server::deleteUser(User &user)
// {
// 	std::map<int, User>::iterator it = usersManage.find(user.getSocket());
// 	if (it != usersManage.end())
// 	{		
// 		close(user.getSocket());
// 		std::cout << RED << "User " << user.getId() << " disconnected" << RESET << std::endl;
// 		usersManage.erase(it);
// 		_activeUsers--;
// 	}
// 	for (int j = user.getId(); j <= _activeUsers; ++j)
// 		_fds[j] = _fds[j + 1];	
// }

void	User::setVerif() {
	_verified = true;
}

void User::setChannel(Channel &channel)
{
    // Si on quitte un channel (channel vide passé)
    if (channel.getName().empty())
    {
        _activeChannel.clear();
        return;
    }
    
    // Si on rejoint un nouveau channel
    _activeChannel = channel.getName();
}

void User::appendPartialCommand(const std::string& partial) {
        _partialCommand += partial;
}

std::string User::getAndClearPartialCommand()
{
    std::string temp = _partialCommand;
    _partialCommand.clear();
    return temp;
}
