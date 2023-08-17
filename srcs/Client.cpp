/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:41:30 by adegain           #+#    #+#             */
/*   Updated: 2023/08/17 16:18:54 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(pollfd* _pfd)
	: pfd(_pfd), has_pwd(false), has_authd(false), nick("*"), user(nick), host(nick), realName(nick)
{
	modes.s = true;
	modes.i = modes.o = false;
}

Client::~Client()
{
}

pollfd*	Client::getPfd()
{
	return pfd;
}

int	Client::getFd()
{
	return pfd->fd;
}

bool	Client::hasPwd()
{
	return has_pwd;
}

void	Client::hasPwd(bool _has_pwd)
{
	has_pwd = _has_pwd;
}

bool	Client::hasAuthd()
{
	return has_authd;
}

void	Client::hasAuthd(bool _has_authd)
{
	has_authd = _has_authd;
}

string	Client::getNick()
{
	return nick;
}

void	Client::setNick(const string& _nick)
{
	nick = _nick;
}

string	Client::getUser()
{
	return user;
}

void	Client::setUser(const string& _user)
{
	user = _user;
}

string	Client::getHost()
{
	return host;
}

void	Client::setHost(const string& _host)
{
	host = _host;
}

string	Client::getRealName()
{
	return realName;
}

void	Client::setRealName(const string& _real_name)
{
	realName = _real_name;
}

clientMode_t*	Client::getMode()
{
	return &modes;
}

string	Client::getModeString()
{
	string	ret = "+";

	if (modes.i)
		ret += "i";
	if (modes.s)
		ret += "s";
	if (modes.o)
		ret += "o";
	return ret;
}

void	Client::setMode(vector<string>& words)
{
	size_t			i;
	parsedMode_t	pm = parsed_mode(word[1], true);
	vector<string>	args;
	string			changes = pm.sign ? "+" : "-";

	if (!pm.valid)
		RFC1459_ERR_UNKNOWNMODE(this, pm.invalid_mode);
	else
	{
		for (i = 0; i < pm.modes.size(); ++i)
		{
			if (pm.modes[i] == 'i')
				modes.i = pm.sign;
			else if (pm.modes[i] == 's')
				modes.s = pm.sign;
			else if (pm.modes[i] == 'o' && !pm.sign)
			{
				modes.o = false;
				changes += pm.modes[i];
			}
			if (pm.modes[i] != 'o')
				changes += pm.modes[i];
		}
		if (changes.size() - 1)
		{
			args.push_back(nick);
			args.push_back(nick);
			args.push_back(changes);
			server->sendString(this, format(RPL_USER_MODE, args));
		}
	}
}