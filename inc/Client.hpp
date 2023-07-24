/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:29:27 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 14:35:00 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
	private:
		Client();
		~Client();

		std::string	_firstName;
		std::string	_lastName;
		std::string	_nickName;
		int			id;

	public:
		void	setNickname(std::string Nickname);
		void	setFisrtName(std::string FisrtName);
		void	setLastName(std::string LastName);
		void	setUserName(std::string UserName);
		void	setStatus();
		void	setModeLevel();

		void	addChannel();
		void	addInviteChannel();
		void	removeChannel();

		const std::string	getNickname();
		const std::string	getStatus();
		void				getChannelList();
};

#endif