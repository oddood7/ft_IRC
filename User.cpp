/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:24:51 by lde-mais          #+#    #+#             */
/*   Updated: 2024/05/28 16:31:52 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _socket(-1), _id(0), _authenticate(false), _dc(false) {}

User::User(int sock, sockaddr_in addr) : _socket(sock), _id(0), \
	 _addr(addr), _host("localhost"), _authenticate(false), _dc(false) {
	}

User::User(const User &cpy) : _socket(cpy._socket), _id(cpy._id), \
	_addr(cpy._addr), _nickName(cpy._nickName), _userName(cpy._userName), _host(cpy._host), _pass(cpy._pass), _rpl(cpy._rpl), \
	_buf(cpy._buf), _authenticate(cpy._authenticate), _dc(cpy._dc) {
	}

User &User::operator=(const User &cpy)
{
    if (this != &cpy)
    {
        _socket = cpy._socket;
        _id = cpy._id;
        _addr = cpy._addr;
        _nickName = cpy._nickName;
        _userName = cpy._userName;
        _host = cpy._host;
        _pass = cpy._pass;
        _rpl = cpy._rpl;
        _buf = cpy._buf;
        _authenticate = cpy._authenticate;
        _dc = cpy._dc;
    }
    return *this;
}

User::~User() {}

void User::setBuf(const std::string buffer)
{
    std::istringstream iss(buffer);
    std::string token;

    while (iss >> token)
    {
        _buf.push_back(token);
    }
}

