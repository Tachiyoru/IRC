/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:18:46 by sleon             #+#    #+#             */
/*   Updated: 2023/09/12 11:53:23 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RFC1459.hpp"
#include "../inc/Server.hpp"

void	RFC1459_JOIN(Client *sender, parsedCmd_t &pc)
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
	parsedCmd_t		pc2;

	if (!pc.has_words)
	{
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
		return;
	}
	canals = split(pc.words[0], ",");
	if (pc.words.size() > 1)
		key = split(pc.words[0], ",");
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
		pc2.has_words = true;
		pc2.words.clear();
		pc2.words.push_back(canals[i]);
		pc2.has_suffix = false;
		if (!chan->getTopic().empty())
			RFC1459_TOPIC(sender, pc2);
		RFC1459_NAMES(sender, pc2);
	}
}
