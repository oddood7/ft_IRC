/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lde-mais <lde-mais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:37:00 by lde-mais          #+#    #+#             */
/*   Updated: 2024/06/17 17:45:34 by lde-mais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::string _owner;
		std::string _pass;

		bool	_i;
		bool	_t;

		size_t	_l;

		std::vector<std::string> _operator;
		std::vector<std::string> _invite;
		std::vector<std::string> _chatters;

	public:
		Channel();
		Channel(std::string name, std::string owner);
		Channel(const Channel& cpy);
		~Channel();

		Channel & operator=(Channel const & cpy);

		std::string	getName() const {return _name;};
		std::string	getTopic() const {return _topic;};
		std::string	getOwner() const {return _owner;};
		std::string	getPass() const {return _pass;};
		bool	getI() const {return _i;};
		bool	getT() const {return _t;};
		size_t	getL() const {return _l;};
		std::vector<std::string>	&getOperator() {return _operator;};
		std::vector<std::string>	&getInvite() {return _invite;};
		std::vector<std::string>	&getChatters() {return _chatters;};

		void	setName(std::string str) {_name = str;};
		void	setTopic(std::string str) {_topic = str;};
		void	setOwner(std::string str) {_owner = str;};
		void	setPass(std::string str) {_pass = str;};
		void	setI(bool boo) {_i = boo;};
		void	setT(bool boo) {_t = boo;};
		void	setL(size_t i) {_l = i;};
};

#endif 