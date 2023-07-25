/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:37:45 by adegain           #+#    #+#             */
/*   Updated: 2023/07/25 13:06:11 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <map>
# include <iostream>
# include "Client.hpp"

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
