/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:37:45 by adegain           #+#    #+#             */
/*   Updated: 2023/08/17 16:04:32 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <map>
# include <iostream>
# include "Client.hpp"
#include <sys/poll.h>
#include <netdb.h>

class Client;
class Channel;

class Server
{
	private:

		const std::string	_serverName;
		const std::string	_password;

		int					_socket;
		int					_state;

		std::list<Client>	_clients;

		std::map<std::string const, Client const> _clientList;
		// std::map<std::string const, Channel const> _channelList;

		typedef void (*fct)(int);
		static std::map<std::string const, fct const> const _commandList;

		void	acceptConnexions();
		void	manageClientMsg();
		void	executeCommand();
		void	createChannel();
		void	delChannel();
		void	doesChannelExist();

		void	getClientByFd();
		void	getClientByName();
		void	addClient();
		void	removeClient();

		void	getServerName();
		void	getServerAddress();
		void	getPort();

	public:
		Server();
		~Server();

		bool	init(const int port, const std::string password);
		bool	run();
};

#endif
