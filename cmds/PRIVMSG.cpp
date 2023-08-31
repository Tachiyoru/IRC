/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 15:26:41 by adegain           #+#    #+#             */
/*   Updated: 2023/08/31 16:05:38 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RFC1459.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"

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

void	RFC1459_PRIVMSG(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("PRIVMSG", sender);
	vector<string>	recipients;
	string			dup = "";
	size_t			i;
	Channel*		chan;
	Client*			cli;
CHANOPRIVSNEEDED
	if (!pc.has_words)
		server->sendString(sender, format(ERR_NORECIPIENT, args));
	else if (!pc.has_suffix)
		server->sendString(sender, format(ERR_NOTEXTTOSEND, args));
	else
	{
		recipients = split(pc.words[0], ",");
		if (has_duplicate_recipient(recipients, dup))
		{
			args.push_back(dup);
			server->sendString(sender, format(ERR_TOOMANYTARGETS, args));
			return;
		}
		for (i = 0; i < recipients.size(); i++)
		{
			if (recipients[i][0] == '&' || recipients[i][0] == '#')
			{
				chan = server->findChannel(recipients[i]);
				{
					if (!chan || (chan->getMode()->n && !chan->isConnected(sender)))
					{
						args.push_back(recipients[i]);
						server->sendString(sender, format(ERR_CANNOTSENDTOCHAN, args));
						continue;
					}
					if (chan->getMode()->m && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
					{
						RFC1459_ERR_CHANOPRIVSNEEDED(sender, recipients[i]);
						continue;
					}
					args.clear();
					args.push_back(sender->getNick());
					args.push_back(sender->getUser());
					args.push_back(sender->getHost());
					args.push_back(recipients[i]);
					args.push_back(pc.suffix);
					chan->broadcast(format(RPL_PRIVMSG, args), sender);
				}
			}
			else
			{
				cli = server->findClient(recipients[i]);
				if (!cli)
				{
					RFC1459_ERR_NOSUCHNICK(sender, recipients[i]);
					continue;
				}
				args.clear();
				args.push_back(sender->getNick());
				args.push_back(sender->getUser());
				args.push_back(sender->getHost());
				args.push_back(recipients[i]);
				args.push_back(pc.suffix);
				server->sendString(cli, format(RPL_PRIVMSG, args));
			}
		}
	}
}