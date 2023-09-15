/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:41:30 by adegain           #+#    #+#             */
/*   Updated: 2023/09/15 22:20:42 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
	: _pfd(), _has_pwd(false), _has_authd(false), _nick("*"), _user(_nick), _host(_nick), _realName(_nick),
	_addr(), _socket(-1)
{
	_modes.s = true;
	_modes.i = _modes.o = false;
}

Client::Client(pollfd* pfd)
	: _pfd(pfd), _has_pwd(false), _has_authd(false), _nick("*"), _user(_nick), _host(_nick), _realName(_nick)
{
	_modes.s = true;
	_modes.i = _modes.o = false;
}

Client::~Client()
{
}

void	Client::setAddr(sockaddr_in addr)
{
	this->_addr = addr;
}
void	Client::setSocket(int socket)
{
	this->_socket = socket;
}

pollfd*	Client::getPfd()
{
	return _pfd;
}

int	Client::getFd()
{
	return _pfd->fd;
}

bool	Client::hasPwd()
{
	return _has_pwd;
}

void	Client::hasPwd(bool has_pwd)
{
	_has_pwd = has_pwd;
}

bool	Client::hasAuthd()
{
	return _has_authd;
}

void	Client::hasAuthd(bool has_authd)
{
	_has_authd = has_authd;
}

string	Client::getNick()
{
	return _nick;
}

void	Client::setNick(const string& nick)
{
	_nick = nick;
}

string	Client::getUser()
{
	return _user;
}

void	Client::setUser(const string& user)
{
	_user = user;
}

string	Client::getHost()
{
	return _host;
}

void	Client::setHost(const string& host)
{
	_host = host;
}

string	Client::getRealName()
{
	return _realName;
}

void	Client::setRealName(const string& real_name)
{
	_realName = real_name;
}

clientMode_t*	Client::getMode()
{
	return &_modes;
}

string	Client::getModeString()
{
	string	ret = "+";

	if (_modes.i)
		ret += "i";
	if (_modes.s)
		ret += "s";
	if (_modes.o)
		ret += "o";
	return ret;
}

void	Client::setMode(vector<string>& words)
{
	size_t			i;
	parsedMode_t	pm = parse_mode(words[1], true);
	vector<string>	args;
	string			changes = pm.sign ? "+" : "-";

	if (!pm.valid)
		RFC1459_ERR_UNKNOWNMODE(this, pm.invalidMode);
	else
	{
		for (i = 0; i < pm.modes.size(); ++i)
		{
			if (pm.modes[i] == 'i')
				_modes.i = pm.sign;
			else if (pm.modes[i] == 's')
				_modes.s = pm.sign;
			else if (pm.modes[i] == 'o' && !pm.sign)
			{
				_modes.o = false;
				changes += pm.modes[i];
			}
			if (pm.modes[i] != 'o')
				changes += pm.modes[i];
		}
		if (changes.size() - 1)
		{
			args.push_back(_nick);
			args.push_back(_nick);
			args.push_back(changes);
			server->sendString(this, format(RPL_USER_MODE, args));
		}
	}
}
