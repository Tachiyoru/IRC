/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:29:27 by adegain           #+#    #+#             */
/*   Updated: 2023/08/25 14:26:34 by sleon            ###   ########.fr       */
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
		sockaddr_in		_addr;
		int				_socket;

	public:
		Client(sockaddr_in const &addr = sockaddr_in(), int sockfd = -1);
		Client(pollfd* _pfd);
		~Client();

		void			setAddr(sockaddr_in addr);
		void			setSocket(int socket);
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
