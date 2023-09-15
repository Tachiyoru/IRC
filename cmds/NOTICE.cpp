/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 15:05:01 by adegain           #+#    #+#             */
/*   Updated: 2023/09/15 19:17:49 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

static bool	has_duplicate_recipient(const vector<string>& rec, string& dup)
{
	for (size_t i = 0; i < rec.size(); i++)
	{
		for (size_t j = 0; j < rec.size(); j++)
		{
			if (rec[i] == rec[j] && i != j)
			{
				dup = rec[i];
				return true;
			}
		}
	}
	return false;
}

void	RFC1459_NOTICE(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("NOTICE", sender);
	vector<string>	recipients;
	string			dup = "";
	size_t			i;
	Channel*		chan;
	Client*			cli;

	if (!pc.has_words || !pc.has_suffix)
		return;
	else
	{
		recipients = split(pc.words[0], ",");
		if (has_duplicate_recipient(recipients, dup))
			return;
		for (i = 0; i < recipients.size(); i++)
		{
			if (recipients[i][0] == '&' || recipients[i][0] == '#')
			{
				chan = server->findChannel(recipients[i]);
				if (!chan || (chan->getMode()->n && !chan->isConnected(sender)))
					continue;
				if (chan->getMode()->m && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
					continue;
				args.clear();
				args.push_back(sender->getNick());
				args.push_back(sender->getUser());
				args.push_back(sender->getHost());
				args.push_back(recipients[i]);
				args.push_back(pc.suffix);
				chan->broadcast(format(RPL_NOTICE, args), sender);
			}
			else
			{
				cli = server->findClient(recipients[i]);
				if (!cli)
					continue;
				args.clear();
				args.push_back(sender->getNick());
				args.push_back(sender->getUser());
				args.push_back(sender->getHost());
				args.push_back(recipients[i]);
				args.push_back(pc.suffix);
				server->sendString(cli, format(RPL_NOTICE, args));
			}
		}
	}
}
