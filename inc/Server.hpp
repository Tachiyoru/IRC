/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:37:45 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 14:38:54 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <iostream>

class Server
{
	private:
		const std::string _serverName;
		const std::string _password;
		
		Server();
		~Server();
		
	public:
		void	start(const int port, const std::string password);
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
};

#endif
