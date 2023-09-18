/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:17:23 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 18:39:01 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	RFC1459_USER(Client *sender, parsedCmd_t &pcmd)
{
	if (!sender->hasPwd() || !sender->getNick().compare("*"))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("USER", sender);
	stringstream	ss;
	string	date, time;

	if (sender->hasAuthd())
		RFC1459_ERR_ALREADYREGISTRED(sender);
	else if (!pcmd.has_words || pcmd.words.size() != 3 || !pcmd.has_suffix)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		sender->setUser(pcmd.words[0]);
		sender->setHost(pcmd.words[1]);
		sender->setRealName(pcmd.suffix);
		args.push_back(sender->getNick());
		server->sendString(sender, format(RPL_WELCOME, args));
		REFRESH_REPLY(sender);
		ss<<server->getPort();
		args.push_back(ss.str());
		server->sendString(sender, format(RPL_YOURHOST, args));
		date = getDateAndTime(1, 0);
		time = getDateAndTime(0, 1);
		REFRESH_REPLY(sender);
		args.push_back(date);
		args.push_back(time);
		server->sendString(sender, format(RPL_CREATED, args));
		REFRESH_REPLY(sender);
		args.push_back(SERVER_NAME);
		args.push_back(RFC1459_USER_MODES);
		args.push_back(RFC1459_CHANNEL_MODES);
		server->sendString(sender, format(RPL_MYINFO, args));

		RFC1459_MOTD(sender, pcmd);
		sender->hasAuthd(true);
	}
}
