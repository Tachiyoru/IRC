/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:51:58 by adegain           #+#    #+#             */
/*   Updated: 2023/08/31 15:43:21 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"

void	RFC1459_LIST(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("LIST", sender);
	vector<string>					canals;
	size_t							i;
	map<string, Channel*>			*channels;
	map<string, Channel*>::iterator	it;
	channelMode_t*					cm;
	Channel*						chan;

	server->sendString(sender, format(RPL_LISTSTART, args));
	if (!pc.has_words)
	{
		channels = server->getChannels();
		for (it = channels->begin(); it != channels->end(); it++)
		{
			cm = it->second->getMode();
			REFRESH_REPLY(sender);
			if (it->second->isConnected(sender) || (!cm->p && !cm->s))
			{
				args.push_back(it->second->getName());
				args.push_back(it->second->getVisibleCount());
				args.push_back(it->second->getTopic());
			}
			else if (cm->p)
			{
				args.push_back("Prv");
				args.push_back(it->second->getVisibleCount());
				args.push_back("");
			}
			else
				continue;
			server->sendString(sender, format(RPL_LIST, args));
		}
	}
	else
	{
		canals = split(pc.words[0], ",");
		for (i = 0; i < canals.size(); i++)
		{
			chan = server->findChannel(canals[i]);
			if (!chan)
				continue;
			cm = chan->getMode();
			REFRESH_REPLY(sender);
			if (chan->isConnected(sender) || (!cm->p && !cm->s))
			{
				args.push_back(it->second->getName());
				args.push_back(it->second->getVisibleCount());
				args.push_back(it->second->getTopic());
			}
			else if (cm->p)
			{
				args.push_back("Prv");
				args.push_back(chan->getVisibleCount());
				args.push_back("");
			}
			else
				continue;
			server->sendString(sender, format(RPL_LIST, args));
		}
	}
	REFRESH_REPLY(sender);
	server->sendString(sender, format(RPL_LISTEND, args));
}