/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:29:27 by adegain           #+#    #+#             */
/*   Updated: 2023/09/22 13:52:57 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <string>
# include <vector>
#include <iostream>
#include "Server.hpp"

using namespace std;

typedef struct clientMode_s
{
	bool	i, s, o;
} clientMode_t;

class Client
{
	private:
		struct pollfd*	_pfd;
		bool			_has_pwd, _has_authd;
		std::string		_nick, _user, _host, _realName;
		clientMode_t	_modes;
		sockaddr_in		_addr;
		int				_socket;

	public:
		Client();
		Client(pollfd* pfd);
		~Client();

		void			setAddr(sockaddr_in addr);
		void			setSocket(int socket);

		pollfd*			getPfd();
		int				getFd();
		bool			hasPwd();
		void			hasPwd(bool has_pwd);
		bool			hasAuthd();
		void			hasAuthd(bool has_authd);
		string			getNick();
		void			setNick(const string& nick);
		string			getUser();
		void			setUser(const string& user);
		string			getHost();
		void			setHost(const string& host);
		string			getRealName();
		void			setRealName(const string& real_name);
		clientMode_t*	getMode();
		string			getModeString();
		void			setMode(vector<string>& words);
};

#endif
