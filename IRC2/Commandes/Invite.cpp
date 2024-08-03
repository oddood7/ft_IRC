#include "../Server.hpp"
#include "../Utils.hpp"

void	Server::invite(User &user)
{
	std::string err;
	if (user.getBuf().size() < 3){
		err = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	if (is_op(user, user.getNickName()) == -1){
		err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	if (isInChannel(user, user.getBuf()[2])){
		err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	if (searchUserChannel(user) != 1){
		err = ERR_NOSUCHNICK(user.getBuf()[1]);
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}
	size_t i;
	for (i = 0; i < getUserChannel(user.getChannel()).getChatters().size(); i++) {
		if (user.getBuf()[1] == getUserChannel(user.getChannel()).getChatters()[i])
			break ;
	}
	if (i != getUserChannel(user.getChannel()).getChatters().size()) {
		err = ERR_USERONCHANNEL(user.getBuf()[1], user.getBuf()[2]);
		send(user.getSocket(), err.c_str(), err.size(), 0);
		return ;
	}

	getUserChannel(user.getChannel()).getInvite().push_back(user.getBuf()[1]);
	err = RPL_INVITING(user.getBuf()[2], user.getBuf()[1]);
	send(user.getSocket(), err.c_str(), err.size(), 0);
}