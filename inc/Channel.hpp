/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:52:25 by adegain           #+#    #+#             */
/*   Updated: 2023/07/24 14:35:23 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
	private:
		Channel();
		~Channel();

	public:
		void	setLogMsg();
		void	setTopic();
		void	setPassword();
		void	setSecured();
		void	setMode();
		void	setMaxUser();
		
		void	addUser();
		void	addModo();
		void	remUser();
		void	remOperator();
		

		void	sendMsg();
		void	sendMode();
		void	sendTopic();
		void	sendMsgToChannel();

		const std::string	getName();
		const std::string	getTopic();
		const int			getUserNumber();
};

#endif