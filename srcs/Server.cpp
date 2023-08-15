/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 15:55:30 by sleon             #+#    #+#             */
/*   Updated: 2023/08/15 17:47:16 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdio.h>

// ************************************************************************** //
//                        Constructors / Destructors                          //
// ************************************************************************** //

Server::Server() : _serverName("Anne.Shan"), _password(), _socket(-1), _state(1),
	_pollfds(){}

Server::~Server() {}

// ************************************************************************** //
//                              Public functions                              //
// ************************************************************************** //

bool	Server::run()
{
	this->_state = 0;
	// std::cout<<"normalement ca tourne"<<std::endl;
	while (!this->_state)
	{
		if (!this->_state || !this->welcoming())
		{
			this->stop();
			return 1;
		}
	}
	return 0;
}

void	Server::stop()
{
	std::cout<<"Server shutdown initiation"<<std::endl;
	if (this->_socket != -1)
		close(this->_socket);
	this->_socket = 1;
	this->_state = 1;
	// this->_channelList.clear();
	// this->_clientList.clear();
	// this->_commandList.clear();
}

bool	Server::init(const int port, const std::string password)
{
	sockaddr_in addr;

	this->_password = password;
	this->_port = port;
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
		return (std::cerr<<"Error: socket: creation "<<std::endl, 1);
	int flags = fcntl(_socket, F_GETFL, 0); //socket non bloquante je sais pas encore si c'est pertinant ou pas
	if (flags < 0)
	{
		this->stop();
		return (std::cerr<<"Error: socket: fcntl(F_GETFL) call"<<std::endl, 1);
	}
	if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		this->stop();
		return (std::cerr<<"Error: socket: fcntl(F_SETFL) call"<<std::endl, 1);
	}
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	addr.sin_family = AF_INET;
	if (bind(this->_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)))
	{
		this->stop();
		return (std::cerr<<"Error: socket: bind call"<<std::endl, 1);
	}
	if (listen(this->_socket, SOMAXCONN))
	{
		this->stop();
		return (std::cerr<<"Error: socket: listen call"<<std::endl, 1);
	}
	pollfd pollfd_1;
	pollfd_1.fd = this->_socket;
	pollfd_1.events = POLLIN | POLLOUT;
	_pollfds.push_back(pollfd_1);
	// _commandList["PASS"] = &fct_PASS;
	// _commandList["NICK"] = &fct_NICK;
	// _commandList["USER"] = &fct_USER;
	// _commandList["MOTD"] = &fct_MOTD;
	// _commandList["TIME"] = &fct_TIME;
	// _commandList["PING"] = &fct_PING;
	// _commandList["QUIT"] = &fct_QUIT;
	// _commandList["TOPIC"] = &fct_TOPIC;
	// _commandList["NAMES"] = &fct_NAMES;
	// _commandList["JOIN"] = &fct_JOIN;
	// _commandList["PART"] = &fct_PART;
	// _commandList["MODE"] = &fct_MODE;
	// _commandList["OPER"] = &fct_OPER;
	// _commandList["KILL"] = &fct_KILL;
	// _commandList["KICK"] = &fct_KICK;
	// _commandList["PRIVMSG"] = &fct_PRIVMSG;
	// _commandList["NOTICE"] = &fct_NOTICE;
	// _commandList["INVITE"] = &fct_INVITE;
	// _commandList["LIST"] = &fct_LIST;
	return 0;
}

