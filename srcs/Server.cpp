/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 15:55:30 by sleon             #+#    #+#             */
/*   Updated: 2023/09/25 14:55:22 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// ************************************************************************** //
//                        Constructors / Destructors                          //
// ************************************************************************** //

Server::Server(int port, string pwd) : _password(pwd), _port(port), _socket(-1), _size(0), _stop(false)
{
	bzero(_pollfds, sizeof(_pollfds));
}

Server::~Server()
{
	size_t i;
	map<string, Channel*>::iterator it;

	// close server
	close(_pollfds[0].fd);
	_pollfds[0].fd = -1;

	// close clients
	for (i = 1; i < _size; ++i)
		disconnectClient(_pollfds + i);

	// close channels
	for (it = _channelList.begin(); it != _channelList.end(); ++it)
		delete it->second;
}

// ************************************************************************** //
//                               Geter functions                              //
// ************************************************************************** //

int	Server::getPort()
{
	return this->_port;
}


std::string	Server::getPassword()
{
	return this->_password;
}

int	Server::getSocket()
{
	return this->_socket;
}

pollfd* Server::getPfd(int i)
{
	if (i < 0 || i >= SOMAXCONN)
		return NULL;
	return this->_pollfds +i;
}

size_t	Server::getSize()
{
	return _size;
}

bool	Server::getStop()
{
	return _stop;
}

void	Server::setStop(bool stop)
{
	_stop = stop;
}

bool	Server::start()
{
	struct addrinfo hints, *ai, *p;
	int opt = 1;
	std::string hostname = "0.0.0.0";
	stringstream ss;
	ss << _port;
	string port_str = ss.str();
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((getaddrinfo(hostname.c_str(), port_str.c_str(), &hints, &ai)) != 0)
	{
		std::cerr<<"getaddrinfo failed"<<std::endl;
		return false;
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr<<"Socket failed"<<std::endl;
			return false;
		}
		if (_socket < 0)
			continue;
		if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		{
			std::cerr<<"Setsockopt Failed"<<std::endl;
			return false;
		}
		if (bind(_socket, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(_socket);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
		return false;

	SERVER_ASSERT(listen(_socket, SOMAXCONN));
	_pollfds[_size].fd = _socket;
	_pollfds[_size].events = POLLIN;
	_size++;

	_cmdsList["PASS"] = &RFC1459_PASS;
	_cmdsList["NICK"] = &RFC1459_NICK;
	_cmdsList["USER"] = &RFC1459_USER;
	_cmdsList["TIME"] = &RFC1459_TIME;
	_cmdsList["PING"] = &RFC1459_PING;
	_cmdsList["QUIT"] = &RFC1459_QUIT;
	_cmdsList["TOPIC"] = &RFC1459_TOPIC;
	_cmdsList["NAMES"] = &RFC1459_NAMES;
	_cmdsList["JOIN"] = &RFC1459_JOIN;
	_cmdsList["PART"] = &RFC1459_PART;
	_cmdsList["MODE"] = &RFC1459_MODE;
	_cmdsList["OPER"] = &RFC1459_OPER;
	_cmdsList["KILL"] = &RFC1459_KILL;
	_cmdsList["KICK"] = &RFC1459_KICK;
	_cmdsList["PRIVMSG"] = &RFC1459_PRIVMSG;
	_cmdsList["NOTICE"] = &RFC1459_NOTICE;
	_cmdsList["INVITE"] = &RFC1459_INVITE;
	_cmdsList["LIST"] = &RFC1459_LIST;

	return true;
}

void	Server::acceptClient()
{
	if (_size + 1 >= SOMAXCONN)
	{
		std::cerr<<_serverName<<": cannot accept another client"<<std::endl;
		return;
	}
	_pollfds[_size].fd = accept(_socket, NULL, NULL);
	if (_pollfds[_size].fd == -1)
	{
		std::cerr<<_serverName<<": could not accept client"<<std::endl;
		return;
	}
	std::cout<<_serverName<<": connection accepted"<<_pollfds[_size].fd<<std::endl;
	_pollfds[_size].events = POLLIN;
	_clientList[_pollfds[_size].fd] = new Client(_pollfds + _size);
	++_size;
}

void	Server::handleRequest(pollfd* pfd)
{
	int		retval;
	char	buf[RFC1459_MAX];
	vector<string>	cmdvec;
	parsedCmd_t		parsed;
	size_t			i;
	static string	bufs[SOMAXCONN];
	bzero(buf, sizeof(buf));

	retval = recv(pfd->fd, buf, sizeof(buf), 0);
	if (retval == -1)
		return;
	else if (!retval)
	{
		disconnectClient(pfd);
		return;
	}

	bufs[pfd->fd] += buf;
	if (bufs[pfd->fd].find(RFC1459_END) == std::string::npos)
		return;

	cout<<"("<<pfd->fd<<") received :\n"<<bufs[pfd->fd]<<std::endl<<std::endl;

// get and iterate received commands
	cmdvec = split(bufs[pfd->fd], RFC1459_END);
	bufs[pfd->fd].clear();
	for (i = 0; i < cmdvec.size(); ++i)
	{
		parsed = parse(cmdvec[i]);
		if (_cmdsList.count(parsed.cmd) == 0)
			;
		else
			_cmdsList[parsed.cmd](_clientList[pfd->fd], parsed);
	}
}

void	Server::disconnectClient(pollfd *pfd)
{
	map<std::string, Channel*>				*channelList = server->getChannelList();
	map<std::string, Channel*>::iterator	it;
	parsedCmd_t								pc;
	std::string								pc_word;

	if (pfd->fd != -1 && _clientList.count(pfd->fd) != 0)
	{
		std::cout<<_serverName<<": lost connection "<< pfd->fd<<std::endl;
		for (it = channelList->begin(); it != channelList->end(); it++)
		{
			if (it->second->isConnected(_clientList[pfd->fd]))
			{
				pc_word += it->second->getName();
				pc_word += ",";
			}
		}
		pc.has_words = true;
		pc.words.push_back(pc_word);
		RFC1459_PART(_clientList[pfd->fd], pc);
		delete _clientList[pfd->fd];
		_clientList.erase(pfd->fd);
		close(pfd->fd);
		pfd->fd = -1;
	}
}

void	Server::sendString(Client *c, const std::string &a)
{
	std::cout<<"("<<c->getFd()<<") "<<"sent:\n"<<a<<std::endl;
	send(c->getFd(), a.c_str(), a.size(), 0);
}

Client*	Server::findClient(std::string name)
{
	map<int, Client*>::iterator	it;

	for (it = _clientList.begin(); it != _clientList.end(); ++it)
	{
		if (it->second->getNick() == name)
			return it->second;
	}
	return NULL;
}

Channel* Server::findChannel(std::string name)
{
	map<std::string, Channel*>::iterator	it;

	for (it = _channelList.begin(); it != _channelList.end(); ++it)
	{
		if (it->second->getName() == name)
			return it->second;
	}
	return NULL;
}

Channel* Server::createChannel(std::string name, Channel* ins, Client* owner)
{
	_channelList[name] = ins;
	ins->addOperator(owner);
	return ins;
}

map<std::string, Channel*>* Server::getChannelList()
{
	return &_channelList;
}

map<int, Client*>* Server::getClientList()
{
	return &_clientList;
}

Server* server;
