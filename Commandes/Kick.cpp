#include "../Server.hpp"
#include "../Utils.hpp"

// void	Server::kick(User &user)
// {
// 	std::string err;
// 	if (user.getBuf().size() < 4) {
// 		err = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
// 		send(user.getSocket(), err.c_str(), err.size(), 0);
// 		return ;
// 	}
// 	if (searchUserChannel(user) != 2){
// 		err = ERR_NOSUCHCHANNEL(user.getBuf()[1]);
// 		send(user.getSocket(), err.c_str(), err.size(), 0);
// 		return ;
// 	}
// 	if (isInChannel(user, user.getBuf()[1])) {
// 		err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
// 		send(user.getSocket(), err.c_str(), err.size(), 0);
// 		return ;
// 	}
// 	if (is_op(user, user.getNickName()) == -1){
// 		err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
// 		send(user.getSocket(), err.c_str(), err.size(), 0);
// 		return ;
// 	}
// 	for (size_t i = 0; i < getUserChannel(user.getChannel()).getChatters().size(); i++) {
// 		if (user.getBuf()[2] == getUserChannel(user.getChannel()).getChatters()[i]) {
// 			err = RPL_KICK(getUserChannel(user.getChannel()).getName(), user.getNickName(), user.getBuf()[2], user.getBuf()[3]);
// 			send(user.getSocket(), err.c_str(), err.size(), 0);
// 			for (size_t j = 0; j < usersManage.size(); j++) {
// 				if (user.getBuf()[2] == usersManage[j].getNickName())
// 					usersManage[j].getChannel().clear();
// 			}
// 			getUserChannel(user.getChannel()).getChatters().erase(getUserChannel(user.getChannel()).getChatters().begin() + i);
// 			i--;
// 		}
// 	}
// }

void Server::kick(User &user)
{
    std::string err;
    if (user.getBuf().size() < 4) {
        err = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    if (searchUserChannel(user) != 2) {
        err = ERR_NOSUCHCHANNEL(user.getBuf()[1]);
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    if (isInChannel(user, user.getBuf()[1])) {
        err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    if (is_op(user, user.getNickName()) == -1) {
        err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
        send(user.getSocket(), err.c_str(), err.size(), 0);
        return;
    }
    for (size_t i = 0; i < getUserChannel(user.getChannel()).getChatters().size(); i++) {
        if (user.getBuf()[2] == getUserChannel(user.getChannel()).getChatters()[i]) {
            std::string channelName = getUserChannel(user.getChannel()).getName();
            std::string kickedNick = user.getBuf()[2];
            
            // Message KICK
            err = RPL_KICK(channelName, user.getNickName(), kickedNick, user.getBuf()[3]);
            
            // Message PART pour forcer la mise à jour du client
            std::string partMsg = RPL_PART(user_id(user.getUserName(), user.getNickName()), channelName, user.getBuf()[3]);
            
            // Envoyer les messages KICK et PART à tous les utilisateurs du canal
            for (size_t k = 0; k < getUserChannel(user.getChannel()).getChatters().size(); k++) {
                for (size_t j = 0; j < usersManage.size(); j++) {
                    if (getUserChannel(user.getChannel()).getChatters()[k] == usersManage[j].getNickName()) {
                        send(usersManage[j].getSocket(), err.c_str(), err.size(), 0);
                        send(usersManage[j].getSocket(), partMsg.c_str(), partMsg.size(), 0);
                        break;
                    }
                }
            }
            
            // Retirer l'utilisateur expulsé du canal
            for (size_t j = 0; j < usersManage.size(); j++) {
                if (kickedNick == usersManage[j].getNickName()) {
                    usersManage[j].getChannel().clear();
                    break;
                }
            }
            
            getUserChannel(user.getChannel()).getChatters().erase(getUserChannel(user.getChannel()).getChatters().begin() + i);
            i--;
        }
    }
}