/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:29:27 by adegain           #+#    #+#             */
/*   Updated: 2023/08/17 14:24:23 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
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
		struct pollfd*	pfd;
		bool			has_pwd, has_authd;
		string			nick, user, host, realName;
		clientMode_t	modes;

	public:
		Client(pollfd* _pfd);
		~Client();
		
		pollfd*			getPfd();
		int				getFd();
		bool			hasPwd();
		void			hasPwd(bool _has_pwd);
		bool			hasAuthd();
		void			hasAuthd(bool _has_authd);
		string			getNick();
		void			setNick(const string& _nick);
		string			getUser();
		void			setUser(const string& _user);
		string			getHost();
		void			setHost(const string& _host);
		string			getRealName();
		void			setRealName(const string& _real_name);
		clientMode_t*	getMode();
		string			getModeString();
		void			setMode(vector<string>& words);
};

#endif