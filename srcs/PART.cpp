/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:51:44 by sleon             #+#    #+#             */
/*   Updated: 2023/09/22 15:53:47 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

void RFC1459_PART(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("PART", sender);
	vector<string> canals;
	size_t i;
	Channel* chan;

	if (!pc.has_words)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		canals = split(pc.words[0], ",");

		for (i = 0; i < canals.size(); ++i)
		{
			chan = server->findChannel(canals[i]);

			if (!chan)
			{
				RFC1459_ERR_NOSUCHCHANNEL(sender, canals[i]);
				continue;
			}

			if (!chan->isConnected(sender))
			{
				RFC1459_ERR_NOTONCHANNEL(sender, canals[i]);
				continue;
			}

			args.clear();
			args.push_back(sender->getNick());
			args.push_back(sender->getUser());
			args.push_back(sender->getHost());
			args.push_back(canals[i]);
			chan->broadcast(format(RPL_PART, args), NULL);

			chan->disconnectUser(sender);
		}
	}
}
