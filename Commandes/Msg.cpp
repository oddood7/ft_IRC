#include "../Server.hpp"

int Server::searchUserChannel(User &user)
{	
	for (std::map<int, User>::iterator ite = usersManage.begin(); ite != usersManage.end(); ite++)
	{
		if (ite->second.getNickName() == user.getBuf()[1])
			return 1;
	}
	for (std::vector<Channel>::iterator ite = _channelsList.begin(); ite != _channelsList.end(); ite++)
	{
		if (ite->getName() == user.getBuf()[1])
			return 2;
	}
	return 0;
}

// void	Server::msg(User &user)
// {
// 	std::string ret;
// 	std::string	message;

// 	if (user.getBuf().size() < 3) {
// 		ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
// 		send(user.getSocket(), ret.c_str(), ret.size(), 0);
// 		return ;
// 	}
	
// 	int b = searchUserChannel(user);

// 	if (b == 1)
// 	{
// 		for (size_t i = 2; i < user.getBuf().size(); i++){
// 			message += user.getBuf()[i];
// 			if (i < user.getBuf().size() - 1)
// 				message += " ";
// 		}
// 		size_t j;
// 		for (j = 0; j < usersManage.size(); j++){
// 			if (user.getBuf()[1] ==  usersManage[j].getNickName())
// 				break;
// 		}
// 		ret = RPL_PRIV(user.getNickName(), user.getBuf()[1], message);
// 		send(usersManage[j].getSocket(), ret.c_str(), ret.size(), 0);
// 	}
// 	else if (b == 2)
// 	{
// 		for (size_t i = 2; i < user.getBuf().size(); i++){
// 			message += user.getBuf()[i];
// 			if (i < user.getBuf().size() - 1)
// 				message += " ";
// 		}
// 		ret = RPL_PRIV(user.getNickName(), user.getBuf()[1], message);
// 		send_all(getUserChannel(user.getChannel()), ret, user.getNickName());
// 	}
// 	else {
// 		ret = ERR_NOSUCHNICK(user.getBuf()[1]);
// 		send(user.getSocket(), ret.c_str(), ret.size(), 0);
// 	}
// }

void Server::msg(User &user)
{
    std::string ret;
    std::string message;

    if (user.getBuf().size() < 3) {
        ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
        return;
    }
    
    std::string target = user.getBuf()[1];
    
    // Construire le message
    for (size_t i = 2; i < user.getBuf().size(); i++) {
        message += user.getBuf()[i];
        if (i < user.getBuf().size() - 1)
            message += " ";
    }

    if (target[0] == '#' || target[0] == '&') // Message à un canal
    {
        Channel* channel = NULL;
        for (size_t i = 0; i < _channelsList.size(); i++) {
            if (_channelsList[i].getName() == target) {
                channel = &_channelsList[i];
                break;
            }
        }

        if (!channel) {
            ret = ERR_NOSUCHCHANNEL(target);
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
            return;
        }

        // Vérifier si l'utilisateur est dans le canal
        bool userInChannel = false;
        for (size_t i = 0; i < channel->getChatters().size(); i++) {
            if (channel->getChatters()[i] == user.getNickName()) {
                userInChannel = true;
                break;
            }
        }

        if (!userInChannel) {
            ret = ERR_NOTONCHANNEL(target);
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
            return;
        }

        ret = RPL_PRIV(user.getNickName(), target, message);
        send_all(*channel, ret, user.getNickName());
    }
    else // Message privé à un utilisateur
    {
        size_t j;
        for (j = 0; j < usersManage.size(); j++) {
            if (target == usersManage[j].getNickName())
                break;
        }

        if (j < usersManage.size()) {
            ret = RPL_PRIV(user.getNickName(), target, message);
            send(usersManage[j].getSocket(), ret.c_str(), ret.size(), 0);
        } else {
            ret = ERR_NOSUCHNICK(target);
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
        }
    }
}