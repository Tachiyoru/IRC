/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:52:25 by adegain           #+#    #+#             */
/*   Updated: 2023/08/25 15:34:46 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>

using namespace std;

typedef struct channelMode_s
{
	bool	p, s, i, t, n, m, lbool, kbool;
	int		l;
	string	k;
} channelMode_t;

class Channel
{
	private:
		channelMode_t		_modes;
		string				_name, _topic, _creation;
		map<int, Client*>	_users;
		map<int, Client*>	_operators;
		map<int, Client*>	_invited;
		map<int, Client*>	_voiced;

	public:
		Channel(string name);
		~Channel();

		string			getName();
		string			getTopic();
		void			setTopic(string topic);
		channelMode_t*	getMode();
		bool			onChannel(int client_fd);
		void			addUser(Client* c);
		void			addOperator(Client* c);
		void			addVoiced(Client* c);
		void			removeUser(Client* c);
		void			removeOperator(Client* c);
		void			removeVoiced(Client* c);
		bool			isUser(Client *c);
		bool			isOperator(Client* c);
		bool			isVoiced(Client* c);
		bool			isConnected(Client* c);
		bool			isInvited(Client *c);
		void			addInvite(Client* c);
		void			removeInvitation(Client* c);
		string			getUserList(bool connected);
		int				getSize();
		void			broadcast(const string& s, Client* ignore);
		string			getModeString();
		string			getCreationTime();
		void			setMode(Client* sender, const string& cmd_name, vector<string>& words);
		void			disconnectUser(Client* c);
		string			getVisibleCount();
};

#endif