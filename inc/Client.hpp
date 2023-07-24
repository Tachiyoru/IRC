/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:29:27 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 16:21:20 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client
{
	private:
		std::string	_firstName;
		std::string	_lastName;
		std::string	_userName;

	public:
		Client();
		~Client() {};
		
		void	setFirstName(std::string FirstName);
		void	setLastName(std::string LastName);
		void	setUserName(std::string UserName);
		void	setStatus();
		void	setModeLevel();

		void	addChannel();
		void	addInviteChannel();
		void	removeChannel();

		const std::string	getUserName();
		const std::string	getStatus();
		void				getChannelList();
		
		int	id;
};

#endif