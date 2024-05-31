/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:30:32 by lde-mais          #+#    #+#             */
/*   Updated: 2024/05/28 16:30:54 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <stdexcept>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <poll.h>
# include <map>
# include "User.hpp"
# include "Channel.hpp"

# define MAX_USERS 5

class Server
{
    private:
    
	int _port;
    int _socket;
    int _activeUsers;
    std::string _password;
    bool _shutdown;
    struct sockaddr_in _address;
    struct pollfd _fds[MAX_USERS + 1];
    std::map<int, User> UsersManage;

    void acceptNewConnection();
    void processUserData(int UserIndex);
	bool authenticateUser(const std::string& password){ return password == _password; }

	public:
    
	Server(int port, const std::string& password);
    ~Server();
    void run();
};

#endif
