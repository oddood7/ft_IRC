/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:09:38 by lde-mais          #+#    #+#             */
/*   Updated: 2024/11/24 15:31:42 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../Server.hpp"
// #include "../Utils.hpp"

// int    searchMode(std::string *tab, std::string search, int size)
// {
//     for (int i = 0; i < size; i++){
//         if (search == tab[i])
//             return i;
//     }
//     return -1;
// }

// void    modeInvis(User &user)
// {
//     std::string ret;
//         if (user.getBuf()[2] == "+i" && user.getMode() == false)
//         {
//             user.setMode(true);
//             ret = MODE_USERMSG(user.getNickName(), "+i");
//             send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         }
//         else if (user.getBuf()[2] == "-i" && user.getMode() == true)
//         {
//             user.setMode(false);
//             ret = MODE_USERMSG(user.getNickName(), "-i");
//             send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         }
// }

// void    Server::modeInvite(User &user)
// {
//     std::string ret;
//     if (user.getBuf()[2] == "+i" && getUserChannel(user.getChannel()).getI() == false)
//     {
//         getUserChannel(user.getChannel()).setI(true);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+i");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     else if (user.getBuf()[2] == "-i" && getUserChannel(user.getChannel()).getI() == true)
//     {
//         getUserChannel(user.getChannel()).setI(false);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-i");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
// }

// void    Server::modeTopic(User &user)
// {
//     std::string ret;
//     if (user.getBuf()[2] == "+t" && getUserChannel(user.getChannel()).getT() == false)
//     {
//         getUserChannel(user.getChannel()).setT(true);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+t");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     else if (user.getBuf()[2] == "-t" && getUserChannel(user.getChannel()).getT() == true)
//     {
//         getUserChannel(user.getChannel()).setT(false);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-t");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
// }

// int    Server::modeKey(User &user)
// {
//     std::string ret;
//     if (user.getBuf()[2] == "+k")
//     {
//         if (user.getBuf().size() == 4)
//             getUserChannel(user.getChannel()).setPass(user.getBuf()[3]);
//         else if (user.getBuf().size() > 4) {
//             ret = ERR_BADPASS(user.getHost());
//             send(user.getSocket(), ret.c_str(), ret.size(), 0);
//             return 1;
//         }
//         else {
//             ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
//             send(user.getSocket(), ret.c_str(), ret.size(), 0);
//             return 1;
//         }

//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+k");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     else if (user.getBuf()[2] == "-k")
//     {
//         getUserChannel(user.getChannel()).getPass().clear();
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-k");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     return 0;
// }

// int    Server::modeMaxUser(User &user)
// {
//     std::string ret;
//     if (user.getBuf()[2] == "+l")
//     {
//         if (user.getBuf()[3].size() > 10)
//         {
//             std::string err = ERR_BADMODEL(user.getHost());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return 1;
//         }
//         long long l = std::atoi(user.getBuf()[3].c_str());
//         if (l < 1 || l > 2147483647)
//         {
//             std::string err = ERR_BADMODEL(user.getHost());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return 1;
//         }
//         getUserChannel(user.getChannel()).setL(l);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+l");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     else if (user.getBuf()[2] == "-l")
//     {
//         getUserChannel(user.getChannel()).setL(0);
//         ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-l");
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//     }
//     return 0;
// }

// int    Server::operatorMode(User &user)
// {    
//     std::string ret;
//     if (user.getBuf().size() < 4){
//         ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         return 1;
//     }
//     if (user.getBuf()[2] == "+o" && is_op(user, user.getBuf()[3]) == -1)
//     {
//         for (size_t i = 0; i < getUserChannel(user.getChannel()).getChatters().size(); i++)
//         {
//             if (user.getBuf()[3] ==  getUserChannel(user.getChannel()).getChatters()[i])
//             {
//                 getUserChannel(user.getChannel()).getOperator().push_back(user.getBuf()[3]);
//                 ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+o");
//                 send(user.getSocket(), ret.c_str(), ret.size(), 0);
//                 return 1;
//             }
//         }
//         ret = ERR_USERNOTINCHANNEL(getUserChannel(user.getChannel()).getName() ,user.getBuf()[3]);
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         return 1;
//     }
//     else if (user.getBuf()[2] == "-o" && is_op(user, user.getBuf()[3]) == 1) {
//         for (std::vector<std::string>::iterator ite = getUserChannel(user.getChannel()).getOperator().begin(); ite != getUserChannel(user.getChannel()).getOperator().end(); ite++) {
//             std::cout << *ite << std::endl;
//             std::cout << user.getBuf()[3] << std::endl;
//             if (user.getBuf()[3] == *ite) {
//                 getUserChannel(user.getChannel()).getOperator().erase(ite);
//                 ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-o");
//                 send(user.getSocket(), ret.c_str(), ret.size(), 0);
//                 break;
//             }
//         }
//     }
//     return 0;
// }

// void    Server::mode(User &user)
// {
//     if (user.getBuf().size() == 2 && user.getBuf()[1] == user.getNickName()){
//         std::string mode = "+";
//         if (user.getMode())
//             mode += "i";    
//         std::string ret = RPL_UMODEIS(user.getNickName(), mode);
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         return ;
//     }
//     int search = searchUserChannel(user);
//     if (search == 1)
//         modeInvis(user);
//     if (search == 2)
//     {
//         if (isInChannel(user, user.getBuf()[1])) {
//             std::string err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return ;
//         }
//         if (is_op(user, user.getNickName()) == -1){
//             std::string err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return ;
//         }
//         if (user.getBuf()[2] == "+i" || user.getBuf()[2] == "-i")
//             modeInvite(user);

//         if (user.getBuf()[2] == "+t" || user.getBuf()[2] == "-t")
//             modeTopic(user);

//         if (user.getBuf()[2] == "+k" || user.getBuf()[2] == "-k"){
//             if (modeKey(user))
//                 return ;
//         }

//         if (user.getBuf()[2] == "+l" || user.getBuf()[2] == "-l"){
//             if (modeMaxUser(user))
//                 return ;
//         }

//         if (user.getBuf()[2] == "+o" || user.getBuf()[2] == "-o")
//         {
//             if (operatorMode(user))
//                 return ;
//         }
//     }
// }

#include "../Server.hpp"
#include "../Utils.hpp"

int    searchMode(std::string *tab, std::string search, int size)
{
    for (int i = 0; i < size; i++){
        if (search == tab[i])
            return i;
    }
    return -1;
}

void    modeInvis(User &user)
{
    std::string ret;
        if (user.getBuf()[2] == "+i" && user.getMode() == false)
        {
            user.setMode(true);
            ret = MODE_USERMSG(user.getNickName(), "+i");
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
        }
        else if (user.getBuf()[2] == "-i" && user.getMode() == true)
        {
            user.setMode(false);
            ret = MODE_USERMSG(user.getNickName(), "-i");
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
        }
}

void    Server::modeInvite(User &user)
{
    std::string ret;
    if (user.getBuf()[2] == "+i" && getUserChannel(user.getChannel()).getI() == false)
    {
        getUserChannel(user.getChannel()).setI(true);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+i");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    else if (user.getBuf()[2] == "-i" && getUserChannel(user.getChannel()).getI() == true)
    {
        getUserChannel(user.getChannel()).setI(false);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-i");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
}

void    Server::modeTopic(User &user)
{
    std::string ret;
    if (user.getBuf()[2] == "+t" && getUserChannel(user.getChannel()).getT() == false)
    {
        getUserChannel(user.getChannel()).setT(true);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+t");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    else if (user.getBuf()[2] == "-t" && getUserChannel(user.getChannel()).getT() == true)
    {
        getUserChannel(user.getChannel()).setT(false);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-t");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
}

int    Server::modeKey(User &user)
{
    std::string ret;
    if (user.getBuf()[2] == "+k")
    {
        if (user.getBuf().size() == 4)
            getUserChannel(user.getChannel()).setPass(user.getBuf()[3]);
        else if (user.getBuf().size() > 4) {
            ret = ERR_BADPASS(user.getHost());
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
            return 1;
        }
        else {
            ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
            return 1;
        }

        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+k");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    else if (user.getBuf()[2] == "-k")
    {
        getUserChannel(user.getChannel()).getPass().clear();
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-k");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    return 0;
}

int    Server::modeMaxUser(User &user)
{
    std::string ret;
    if (user.getBuf()[2] == "+l")
    {
        if (user.getBuf()[3].size() > 10)
        {
            std::string err = ERR_BADMODEL(user.getHost());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return 1;
        }
        long long l = std::atoi(user.getBuf()[3].c_str());
        if (l < 1 || l > 2147483647)
        {
            std::string err = ERR_BADMODEL(user.getHost());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return 1;
        }
        getUserChannel(user.getChannel()).setL(l);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+l");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    else if (user.getBuf()[2] == "-l")
    {
        getUserChannel(user.getChannel()).setL(0);
        ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-l");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
    }
    return 0;
}

int    Server::operatorMode(User &user)
{    
    std::string ret;
    if (user.getBuf().size() < 4){
        ret = ERR_NEEDMOREPARAMS(user.getHost(), user.getBuf()[0]);
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
        return 1;
    }
    if (user.getBuf()[2] == "+o" && is_op(user, user.getBuf()[3]) == -1)
    {
        for (size_t i = 0; i < getUserChannel(user.getChannel()).getChatters().size(); i++)
        {
            if (user.getBuf()[3] ==  getUserChannel(user.getChannel()).getChatters()[i])
            {
                getUserChannel(user.getChannel()).getOperator().push_back(user.getBuf()[3]);
                ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "+o");
                send(user.getSocket(), ret.c_str(), ret.size(), 0);
                return 1;
            }
        }
        ret = ERR_USERNOTINCHANNEL(getUserChannel(user.getChannel()).getName() ,user.getBuf()[3]);
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
        return 1;
    }
    else if (user.getBuf()[2] == "-o" && is_op(user, user.getBuf()[3]) == 1) {
        for (std::vector<std::string>::iterator ite = getUserChannel(user.getChannel()).getOperator().begin(); ite != getUserChannel(user.getChannel()).getOperator().end(); ite++) {
            std::cout << *ite << std::endl;
            std::cout << user.getBuf()[3] << std::endl;
            if (user.getBuf()[3] == *ite) {
                getUserChannel(user.getChannel()).getOperator().erase(ite);
                ret = MODE_CHANNELMSG(getUserChannel(user.getChannel()).getName(), "-o");
                send(user.getSocket(), ret.c_str(), ret.size(), 0);
                break;
            }
        }
    }
    return 0;
}

// void    Server::mode(User &user)
// {
//     if (user.getBuf().size() == 2 && user.getBuf()[1] == user.getNickName()){
//         std::string mode = "+";
//         if (user.getMode())
//             mode += "i";    
//         std::string ret = RPL_UMODEIS(user.getNickName(), mode);
//         send(user.getSocket(), ret.c_str(), ret.size(), 0);
//         return ;
//     }
//     int search = searchUserChannel(user);
//     if (search == 1)
//         modeInvis(user);
//     if (search == 2)
//     {
//         if (isInChannel(user, user.getBuf()[1])) {
//             std::string err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return ;
//         }
//         if (is_op(user, user.getNickName()) == -1){
//             std::string err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
//             send(user.getSocket(), err.c_str(), err.size(), 0);
//             return ;
//         }
//         if (user.getBuf()[2] == "+i" || user.getBuf()[2] == "-i")
//             modeInvite(user);

//         if (user.getBuf()[2] == "+t" || user.getBuf()[2] == "-t")
//             modeTopic(user);

//         if (user.getBuf()[2] == "+k" || user.getBuf()[2] == "-k"){
//             if (modeKey(user))
//                 return ;
//         }

//         if (user.getBuf()[2] == "+l" || user.getBuf()[2] == "-l"){
//             if (modeMaxUser(user))
//                 return ;
//         }

//         if (user.getBuf()[2] == "+o" || user.getBuf()[2] == "-o")
//         {
//             if (operatorMode(user))
//                 return ;
//         }
//     }
// }

void Server::mode(User &user)
{
    // Si pas de paramètres
    if (user.getBuf().size() == 1)
    {
        std::string ret = RPL_UMODEIS(user.getNickName(), "+i");
        send(user.getSocket(), ret.c_str(), ret.size(), 0);
        return;
    }
    
    // Si MODE avec juste le nom du channel
    if (user.getBuf().size() == 2)
    {
        std::string target = user.getBuf()[1];
        
        // Si c'est un channel
        if (target[0] == '#')
        {
            for (size_t i = 0; i < _channelsList.size(); ++i)
            {
                if (_channelsList[i].getName() == target)
                {
                    // Construire la string des modes actuels du channel
                    std::string modes = "+";
                    if (_channelsList[i].getI())
                        modes += "i";
                    if (_channelsList[i].getT())
                        modes += "t";
                    if (!_channelsList[i].getPass().empty())
                        modes += "k";
                    if (_channelsList[i].getL() != 0)
                        modes += "l";
                    
                    // Si aucun mode n'est actif, envoyer juste un +
                    if (modes == "+")
                        modes = "+";
                        
                    // Envoyer le RPL_CHANNELMODEIS
                    std::string rpl = ":" + _name + " 324 " + user.getNickName() + " " + target + " " + modes + "\r\n";
                    send(user.getSocket(), rpl.c_str(), rpl.size(), 0);
                    
                    // Envoyer aussi le RPL_CREATIONTIME
                    std::string timeRpl = ":" + _name + " 329 " + user.getNickName() + " " + target + " " + "1\r\n";
                    send(user.getSocket(), timeRpl.c_str(), timeRpl.size(), 0);
                    return;
                }
            }
            // Channel non trouvé
            std::string err = ERR_NOSUCHCHANNEL(target);
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return;
        }
        // Si c'est un utilisateur
        else if (target == user.getNickName())
        {
            std::string mode = "+";
            if (user.getMode())
                mode += "i";
            std::string ret = RPL_UMODEIS(user.getNickName(), mode);
            send(user.getSocket(), ret.c_str(), ret.size(), 0);
            return;
        }
    }
    
    // A partir d'ici, c'est le reste de votre code existant pour les autres cas de MODE
    int search = searchUserChannel(user);
    if (search == 1)
        modeInvis(user);
    if (search == 2)
    {
        if (isInChannel(user, user.getBuf()[1])) {
            std::string err = ERR_NOTONCHANNEL(getUserChannel(user.getChannel()).getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return ;
        }
        if (is_op(user, user.getNickName()) == -1){
            std::string err = ERR_CHANOPRIVSNEEDED(getUserChannel(user.getChannel()).getName());
            send(user.getSocket(), err.c_str(), err.size(), 0);
            return ;
        }
        if (user.getBuf()[2] == "+i" || user.getBuf()[2] == "-i")
            modeInvite(user);

        if (user.getBuf()[2] == "+t" || user.getBuf()[2] == "-t")
            modeTopic(user);

        if (user.getBuf()[2] == "+k" || user.getBuf()[2] == "-k"){
            if (modeKey(user))
                return ;
        }

        if (user.getBuf()[2] == "+l" || user.getBuf()[2] == "-l"){
            if (modeMaxUser(user))
                return ;
        }

        if (user.getBuf()[2] == "+o" || user.getBuf()[2] == "-o")
        {
            if (operatorMode(user))
                return ;
        }
    }
}