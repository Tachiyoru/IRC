/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 14:19:51 by adegain           #+#    #+#             */
/*   Updated: 2023/09/15 22:21:38 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(string name)
	: _name(name), _topic("")
{
	_modes.p = false;
	_modes.s = false;
	_modes.i = false;
	_modes.t = true;
	_modes.n = true;
	_modes.m = false;
	_modes.lbool = false;
	_modes.kbool = false;
	_modes.l = SOMAXCONN;
	_modes.k = "";

	time_t				seconds = time(NULL);
	std::stringstream	ss;
	ss << seconds;
	_creation = ss.str();
}

Channel::~Channel()
{
}

string	Channel::getName()
{
	return _name;
}

string	Channel::getTopic()
{
	return _topic;
}

void	Channel::setTopic(string topic)
{
	_topic = topic;
}

channelMode_t*	Channel::getMode()
{
	return &_modes;
}

bool	Channel::onChannel(int client_fd)
{
	return (_users.count(client_fd) || _operators.count(client_fd) || _voiced.count(client_fd)) ? true : false;
}

void	Channel::addUser(Client* c)
{
	_users[c->getFd()] = c;
	removeOperator(c);
	removeVoiced(c);
}

void	Channel::addOperator(Client* c)
{
	_operators[c->getFd()] = c;
	removeUser(c);
	removeVoiced(c);
}

void	Channel::removeUser(Client* c)
{
	if (_users.count(c->getFd()))
		_users.erase(c->getFd());
}

void	Channel::removeOperator(Client* c)
{
	if (_operators.count(c->getFd()))
		_operators.erase(c->getFd());
}

void	Channel::addVoiced(Client* c)
{
	_voiced[c->getFd()] = c;
	removeUser(c);
	removeOperator(c);
}

void	Channel::removeVoiced(Client* c)
{
	if (_voiced.count(c->getFd()))
		_voiced.erase(c->getFd());
}

bool	Channel::isUser(Client* c)
{
	return _users.count(c->getFd()) != 0;
}

bool	Channel::isOperator(Client* c)
{
	return _operators.count(c->getFd()) != 0;
}

bool Channel::isVoiced(Client* c)
{
	return _voiced.count(c->getFd()) != 0;
}

bool	Channel::isConnected(Client* c)
{
	return (isUser(c) || isOperator(c) || isVoiced(c));
}

bool	Channel::isInvited(Client* c)
{
	return _invited.count(c->getFd()) != 0;
}

void	Channel::addInvite(Client* c)
{
	_invited[c->getFd()] = c;
}

void	Channel::removeInvitation(Client* c)
{
	if (_invited.count(c->getFd()) != 0)
		_invited.erase(c->getFd());
}

string	Channel::getUserList(bool connected)
{
	string	ret = "";
	map<int, Client*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (!connected && it->second->getMode()->i)
			continue;
		ret += it->second->getNick();
		ret += " ";
	}
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (!connected && it->second->getMode()->i)
			continue;
		ret += "@";
		ret += it->second->getNick();
		ret += " ";
	}
	for (it = _voiced.begin(); it != _voiced.end(); it++)
	{
		if (!connected && it->second->getMode()->i)
			continue;
		ret += "+";
		ret += it->second->getNick();
		ret += " ";
	}
	return ret;
}

int	Channel::getSize()
{
	int	ret = 0;
	map<int, Client*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
		ret++;
	for (it = _operators.begin(); it != _operators.end(); it++)
		ret++;
	for (it = _voiced.begin(); it != _voiced.end(); it++)
		ret++;
	return ret;
}

void	Channel::broadcast(const string& s, Client* ignore)
{
	map<int, Client*>::iterator it;
	clientMode_t*	cm;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		cm = it->second->getMode();
		if (cm->s && it->second != ignore)
			server->sendString(it->second, s);
	}
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		cm = it->second->getMode();
		if (cm->s && it->second != ignore)
			server->sendString(it->second, s);
	}
	for (it = _voiced.begin(); it != _voiced.end(); it++)
	{
		cm = it->second->getMode();
		if (cm->s && it->second != ignore)
			server->sendString(it->second, s);
	}
}

string	Channel::getModeString()
{
	string	ret = "+";

	if (_modes.p)
		ret += "p";
	if (_modes.s)
		ret += "s";
	if (_modes.i)
		ret += "i";
	if (_modes.t)
		ret += "t";
	if (_modes.n)
		ret += "n";
	if (_modes.m)
		ret += "m";
	if (_modes.lbool)
		ret += "l";
	if (_modes.kbool && !_modes.k.empty())
		ret += "k";
	return ret;
}

string	Channel::getCreationTime()
{
	return _creation;
}

void	Channel::setMode(Client* sender, const string& cmd_name, vector<string>& words)
{
	size_t			i;
	parsedMode_t		pm = parse_mode(words[1], false);
	vector<string>	args;
	string			changes;
	Client*			target;

	if (!pm.valid)
		RFC1459_ERR_UNKNOWNMODE(sender, pm.invalidMode);
	else if (pm.modes.find_first_of("ov") != string::npos && (pm.modes.size() != 1 || words.size() < 3))
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		for (i = 0; i < pm.modes.size(); i++)
		{
			if (pm.modes[i] == 'o')
			{
				target = server->findClient(words[2]);
				if (!target || !isConnected(target))
				{
					RFC1459_ERR_NOSUCHNICK(sender, words[2]);
					return;
				}
				if (pm.sign)
					addOperator(target);
				else
					addUser(target);
			}
			else if (pm.modes[i] == 'v')
			{
				target = server->findClient(words[2]);
				if (!target || !isConnected(target))
				{
					RFC1459_ERR_NOSUCHNICK(sender, words[2]);
					return;
				}
				if (pm.sign)
					addVoiced(target);
				else
					addUser(target);
			}
			else if (pm.modes[i] == 'l')
			{
				if (pm.sign)
				{
					if (pm.modes.size() != 1 || words.size() < 3)
						RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
					else
						_modes.l = std::atoi(words[2].c_str());
				}
				else
					_modes.l = SOMAXCONN;
				_modes.lbool = pm.sign;
			}
			else if (pm.modes[i] == 'k')
			{
				if (pm.sign)
				{
					if (pm.modes.size() != 1 || words.size() < 3)
						RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
					else
						_modes.k = words[2];
				}
				else
					_modes.k = "";
				_modes.kbool = pm.sign;
			}
			else if (pm.modes[i] == 'p')
				_modes.p = pm.sign;
			else if (pm.modes[i] == 's')
				_modes.p = pm.sign;
			else if (pm.modes[i] == 'i')
				_modes.p = pm.sign;
			else if (pm.modes[i] == 't')
				_modes.p = pm.sign;
			else if (pm.modes[i] == 'n')
				_modes.p = pm.sign;
			else if (pm.modes[i] == 'm')
				_modes.p = pm.sign;
		}
		args.push_back(sender->getNick());
		args.push_back(sender->getUser());
		args.push_back(sender->getHost());
		args.push_back(_name); // -----------------------------------------------------> a verifier
		changes = "-";
		if (pm.sign)
			changes = "+";
		changes += pm.modes;
		args.push_back(changes);
		broadcast(format(RPL_CHANNEL_MODE, args), NULL);
	}
}

void	Channel::disconnectUser(Client* c)
{
	removeUser(c);
	removeOperator(c);
	removeVoiced(c);
	if (!getSize())
	{
		server->getChannelList()->erase(_name); // ---------------------------------------> a verifier
		delete this;
	}
}

string	Channel::getVisibleCount()
{
	stringstream	ss;
	int				count = 0;
	map<int, Client*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (!it->second->getMode()->i)
			count++;
	}
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (!it->second->getMode()->i)
			count++;
	}
	for (it = _voiced.begin(); it != _voiced.end(); it++)
	{
		if (!it->second->getMode()->i)
			count++;
	}
	ss << count;
	return ss.str();
}
