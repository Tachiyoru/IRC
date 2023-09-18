/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:18:18 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 18:38:39 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	RFC1459_MODE(Client *sender, parsedCmd_t &pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}

	REGISTER_COMMAND("MODE", sender);
	Channel	*channel;
	Client	*client;

	if (!pcmd.has_words)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else if (pcmd.words[0][0] == '&' || pcmd.words[0][0] == '#')
	{
		channel = server->findChannel(pcmd.words[0]);
		if (!channel)
			RFC1459_ERR_NOSUCHCHANNEL(sender, pcmd.words[0]);
		else if (!channel->isConnected(sender))
			RFC1459_ERR_NOTONCHANNEL(sender, pcmd.words[0]);
		else if (pcmd.words.size() == 1)
		{
			args.push_back(pcmd.words[0]);
			args.push_back(channel->getModeString());
			server->sendString(sender, format(RPL_CHANNELMODEIS, args));
			REFRESH_REPLY(sender);
			args.push_back(pcmd.words[0]);
			args.push_back(channel->getCreationTime());
			server->sendString(sender, format(RPL_CREATIONTIME, args));
		}
		else if (!channel->isOperator(sender))
			RFC1459_ERR_CHANOPRIVSNEEDED(sender, pcmd.words[0]);
		else
			channel->setMode(sender, cmd_name, pcmd.words);
	}
	else
	{
		client = server->findClient(pcmd.words[0]);
		if (!client)
			RFC1459_ERR_NOSUCHNICK(sender, pcmd.words[0]);
		else if (sender->getNick().compare(pcmd.words[0]))
			server->sendString(sender, format(ERR_USERSDONTMATCH, args));
		else if (pcmd.words.size() == 1)
		{
			args.push_back(client->getModeString());
			server->sendString(sender, format(RPL_UMODEIS, args));
		}
		else
			client->setMode(pcmd.words);
	}
}
