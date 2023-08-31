/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:37:45 by adegain           #+#    #+#             */
/*   Updated: 2023/08/30 14:03:01 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <stdint.h>
# include <string>
# include <strings.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <map>
# include <netdb.h>
# include <sstream>

# include "Channel.hpp"
# include "Client.hpp"
#define SERVER_ASSERT(cond) if ((cond) == -1)    return false
#include "Utils.hpp"
#include <sys/poll.h>
#include <netdb.h>

class Client;
class Channel;

bool	g_status;

class Server
{
	private:

		const std::string								_serverName;
		std::string										_password;
		int												_port;
		int												_socket;
		pollfd											_pollfds[SOMAXCONN];
		size_t											_size;
		bool											_stop;
		std::map<std::string, cmd_t>					_cmdsList;
		std::map<int, Client *>							_clientList;
		std::map<std::string , Channel *>				_channelList;

	public:
		Server();
		~Server();

		int												getPort();
		std::string										getPassword();
		int												getSocket();
		pollfd											*getPfd(int i);
		size_t											getSize();
		bool											getStop();
		void											setStop(bool stop);

		bool											init(int port, char* password);
		bool											start();
		void											acceptClient();
		void											handleRequest(pollfd	*pfd);
		void											disconnectClient(pollfd	*pfd);
		void											sendString(Client *c, const std::string &s);
		Client											*findClient(std::string name);
		Channel											*findChannel(std::string name);
		Channel											*createChannel(std::string name, Channel *src, Client *client);
		std::map<int, Client *>					*getClientList();
		std::map<std::string , Channel *>		*getChannelList();
};

extern Server *server;

#endif
