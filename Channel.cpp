#include "Channel.hpp"

// Constructeur
Channel::Channel(const std::string& name)
    : _name(name), _topic(""), _mode(0), _userLimit(0), _passwordProtected(false) {
}

// Ajouter un utilisateur au canal
void Channel::addUser(const User& user) {
    _users.insert(user);
}

// Retirer un utilisateur du canal
void Channel::removeUser(const User& user) {
    _users.erase(user);
}

// Définir un opérateur du canal
void Channel::setOperator(const User& user, bool isOperator) {
    if (isOperator) {
        _operators.insert(user);
    } else {
        _operators.erase(user);
    }
}

// Définir le sujet du canal
void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

// Définir le mode du canal
void Channel::setMode(int mode) {
    _mode = mode;
}

// Définir la limite d'utilisateurs
void Channel::setUserLimit(int limit) {
    _userLimit = limit;
}

// Activer/Désactiver la protection par mot de passe
void Channel::setPasswordProtected(bool passwordProtected) {
    _passwordProtected = passwordProtected;
}

// Vérifier si un utilisateur est opérateur
bool Channel::isOperator(const User& user) const {
    return _operators.count(user) > 0;
}

// Récupérer les utilisateurs du canal
const std::set<User>& Channel::getUsers() const {
    return _users;
}

// Récupérer les opérateurs du canal
const std::set<User>& Channel::getOperators() const {
    return _operators;
}

// Récupérer le sujet du canal
const std::string& Channel::getTopic() const {
    return _topic;
}

// Récupérer le mode du canal
int Channel::getMode() const {
    return _mode;
}

// Récupérer la limite d'utilisateurs
int Channel::getUserLimit() const {
    return _userLimit;
}

// Vérifier si le canal est protégé par un mot de passe
bool Channel::isPasswordProtected() const {
    return _passwordProtected;
}