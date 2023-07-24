/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:39:36 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 14:31:13 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>

class server
{
	private:
		const std::string _serverName;
		const std::string _password;
		server();
		~server();
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

server::server()
{
}

server::~server()
{
}


#endif
