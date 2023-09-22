/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:54:23 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 18:40:15 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	RFC1459_TOPIC(Client *sender, parsedCmd_t &pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("TOPIC", sender);
	Channel	*chan;
	if (!pcmd.has_words)
	{
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
		return;
	}
	chan = server->findChannel(pcmd.words[0]);
	if (!chan)
		RFC1459_ERR_NOSUCHCHANNEL(sender, pcmd.words[0]);
	else
	{
		if (!chan->onChannel(sender->getFd()))
		{
			RFC1459_ERR_NOTONCHANNEL(sender, pcmd.words[0]);
			return;
		}
		if (!pcmd.has_suffix)
		{
			if(chan->getTopic().empty())
			{
				args.push_back(pcmd.words[0]);
				server->sendString(sender, format(RPL_NOTOPIC, args));
				return;
			}
		}
		else
		{
			if (chan->getMode()->t && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
			{
				RFC1459_ERR_CHANOPRIVSNEEDED(sender, pcmd.words[0]);
				return;
			}
			chan->setTopic(pcmd.suffix);
		}
		args.clear();
		args.push_back(sender->getNick());
		args.push_back(sender->getUser());
		args.push_back(sender->getHost());
		args.push_back(pcmd.words[0]);
		args.push_back(chan->getTopic());
		chan->broadcast(format(RPL_TOPIC, args), NULL);
	}
}
