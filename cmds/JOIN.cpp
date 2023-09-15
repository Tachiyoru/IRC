/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:18:46 by sleon             #+#    #+#             */
/*   Updated: 2023/09/15 18:21:00 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RFC1459.hpp"
#include "../inc/Server.hpp"

void	RFC1459_JOIN(Client *sender, parsedCmd_t &pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}

	REGISTER_COMMAND("JOIN", sender);
	vector<string>	canals, key;
	Channel			*chan;
	channelMode_t	*cm;
	parsedCmd_t		pcmd2;

	if (!pcmd.has_words)
	{
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
		return;
	}
	canals = split(pcmd.words[0], ",");
	if (pcmd.words.size() > 1)
		key = split(pcmd.words[0], ",");
	while (key.size() < canals.size())
		key.push_back("");

	for (size_t i = 0; i < canals.size(); i++)
	{
		chan = server->findChannel(canals[i]);
		if (!chan)
			chan = server->createChannel(canals[i], new Channel(canals[i]), sender);
		else
		{
			cm = chan->getMode();
			REFRESH_REPLY(sender);
			if (cm->i)
			{
				if (!chan->isInvited(sender))
				{
					args.push_back(canals[i]);
					server->sendString(sender, format(ERR_INVITEONLYCHAN, args));
					continue;
				}
				chan->removeInvitation(sender);
			}
			if (cm->kbool && !cm->k.empty() && cm->k.compare(key[i]))
			{
				args.push_back(canals[i]);
				server->sendString(sender, format(ERR_BADCHANNELKEY, args));
				continue;
			}
			if (chan->getSize() + 1 > cm->l)
			{
					args.push_back(canals[i]);
					server->sendString(sender, format(ERR_CHANNELISFULL, args));
					continue;
			}
			chan->addUser(sender);
		}
		args.clear();
		args.push_back(sender->getNick());
		args.push_back(sender->getUser());
		args.push_back(sender->getHost());
		args.push_back(canals[i]);
		chan->broadcast(format(RPL_JOIN, args), NULL);
		pcmd2.has_words = true;
		pcmd2.words.clear();
		pcmd2.words.push_back(canals[i]);
		pcmd2.has_suffix = false;
		if (!chan->getTopic().empty())
			RFC1459_TOPIC(sender, pcmd2);
		RFC1459_NAMES(sender, pcmd2);
	}
}
