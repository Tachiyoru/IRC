/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:41:30 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 16:26:03 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


		void	setStatus();
		void	setModeLevel();

		void	addChannel();
		void	addInviteChannel();
		void	removeChannel();

		const std::string	getUserName();
		const std::string	getStatus();
		void				getChannelList();

Client::Client()
{
	std::string	ClientInfo[3];
	const char*	Category [3] = {
		"Enter your first name: ",
		"Enter your last name: ",
		"Enter your username: "
	};
	
	for (int i = 0; i < 3; i++)
	{
		std::cout << Category[i];
		std::cin >> ClientInfo[i];
	}
	void (Client::*ftPtr[3])(std::string) = {
		&Client::setFirstName,
		&Client::setLastName,
		&Client::setUserName,
	};

	for (int i = 0; i < 3; i++)
		(this->*ftPtr[i])(ClientInfo[i]);

	// le serveur doit vérifier que les infos sont valides et id++ si valide
	return ;
}

void	Client::setFirstName(std::string FirstName) {this->_firstName = FirstName;}

void	Client::setLastName(std::string LastName) {this->_lastName = LastName;}

void	Client::setUserName(std::string UserName) {this->_userName = UserName;}

// void	Client::setStatus()
// {
	
// }

// void	Client::setModeLevel()
// {
	
// }

// void	Client::addChannel()
// {
	
// }

// void	Client::addInviteChannel()
// {
	
// }

// void	Client::removeChannel()
// {
	
// }

const std::string	Client::getUserName()
{
	return this->_userName;
}

const std::string	Client::getStatus()
{

}

void	Client::getChannelList()
{
	
}