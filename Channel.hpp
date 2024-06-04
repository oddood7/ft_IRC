#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"
# include <set>
# include <string>

class Channel
{
  private:
	std::string _name;
	std::string _topic;
	int _mode;
	int _userLimit;
	bool _passwordProtected;
	std::set<User> _users;
	std::set<User> _operators;

  public:
	Channel(const std::string &name);

	void addUser(const User &user);
	void removeUser(const User &user);
	void setOperator(const User &user, bool isOperator);

	void setTopic(const std::string &topic);
	void setMode(int mode);
	void setUserLimit(int limit);
	void setPasswordProtected(bool passwordProtected);

	bool isOperator(const User &user) const;
	const std::set<User> &getUsers() const;
	const std::set<User> &getOperators() const;
	const std::string &getTopic() const;
	int getMode() const;
	int getUserLimit() const;
	bool isPasswordProtected() const;
};

#endif