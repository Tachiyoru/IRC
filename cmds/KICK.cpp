/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 15:52:54 by adegain           #+#    #+#             */
/*   Updated: 2023/09/15 19:17:49 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

void	RFC1459_KICK(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("KICK", sender);
	vector<string>	canals, users;
	size_t			i, j;
	Channel*		chan;
	Client*			cli;
	parsedCmd_t		pc2;

	if (!pc.has_words || pc.words.size() < 2)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		canals = split(pc.words[0], ",");
		users = split(pc.words[1], ",");
		for (i = 0; i < canals.size(); i++)
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
			if (!chan->isOperator(sender))
			{
				RFC1459_ERR_CHANOPRIVSNEEDED(sender, canals[i]);
				continue;
			}
			for (j = 0; j < users.size(); j++)
			{
				cli = server->findClient(users[j]);
				if (!cli || !chan->isConnected(cli))
					continue;
				pc2.has_words = true;
				pc2.words.clear();
				pc2.words.push_back(canals[i]);
				RFC1459_PART(cli, pc2);
			}
		}
	}
}
