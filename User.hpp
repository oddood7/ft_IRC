/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:08:05 by lde-mais          #+#    #+#             */
/*   Updated: 2024/05/28 16:31:34 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <netinet/in.h>
#include <vector>
#include <sstream>

class User {
private:
    int _socket;
    int _id;
    struct sockaddr_in _addr;
    std::string _nickName;
    std::string _userName;
    std::string _host;
    std::string _pass;
    std::string _rpl;
    std::vector<std::string> _buf;
    // bool _invisible;
    // bool _irssi;
    bool _authenticate;
    bool _dc;

	public:
    
	User();
    User(int sock, sockaddr_in addr);
    User(const User &cpy);
    User &operator=(const User &cpy);
    ~User();

    int getSocket() const { return _socket; }
    int getId() const { return _id; }
    void setId(int id) { _id = id; }
    std::vector<std::string>& getBuf() { return _buf; }
    void setBuf(const std::string buffer);
    void setAuthenticated(bool authenticated) { _authenticate = authenticated; }
    bool isAuthenticated() const { return _authenticate; }
    void setPass(const std::string& pass) { _pass = pass; }
    const std::string& getPass() const { return _pass; }
};

#endif
