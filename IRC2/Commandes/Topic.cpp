#include "../Server.hpp"

void	Server::topic(User &user)
{
	if (user.getBuf().size() < 2){
		std::string ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
		send(user.getSocket(), ret.c_str(), ret.size(), 0);
	}
	if (isInChannel(user, user.getBuf()[1]))
		return ;
	if (user.getBuf().size() == 2)
	{
		std::string rpl;
		if (getUserChannel(user.getChannel()).getTopic().size() > 0)
			rpl = RPL_TOPIC(getUserChannel(user.getChannel()).getName(), getUserChannel(user.getChannel()).getTopic());
		else
			rpl = RPL_NOTOPIC(getUserChannel(user.getChannel()).getName());
		send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
	}
	else if (user.getBuf().size() > 2)
	{
		if (getUserChannel(user.getChannel()).getT() == true) {
			if (is_op(user, user.getNickName()) == -1)
				return ;
		}
		std::string topic;
		for (size_t i = 2; i < user.getBuf().size(); i++){
			topic += user.getBuf()[i];
			if (i < user.getBuf().size() - 1)
				topic += " ";
		}
		getUserChannel(user.getChannel()).setTopic(topic);
		std::string rpl = RPL_TOPIC(getUserChannel(user.getChannel()).getName(), topic);
		send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
	}
}