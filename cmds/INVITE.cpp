/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegain <adegain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:52:24 by adegain           #+#    #+#             */
/*   Updated: 2023/08/31 15:06:42 by adegain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RFC1459.hpp"
#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"

void	RFC1459_INVITE(Client* sender, parsedCmd_t& pc)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	} 
	REGISTER_COMMAND("INVITE", sender);
	Client*			cli;
	Channel*		chan;
	channelMode_t*	cm;
	
	if (!pc.has_words || pc.words.size() != 2)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		cli = server->findClient(pc.words[0]);
		chan = server->findChannel(pc.words[1]);
		cm = chan->getMode();
		if (!cli)
			RFC1459_ERR_NOSUCHNICK(sender, pc.words[0]);
		else if (!chan)
			return;
		else if (cm->i && (!chan->isOperator(sender) && !chan->isVoiced(sender)))
			RFC1459_ERR_CHANOPRIVSNEEDED(sender, pc.words[1]);
		else if (!cm->i && !chan->isConnected(sender))
			RFC1459_ERR_NOTONCHANNEL(sender, pc.words[1]);
		else if (chan->isConnected(cli))
		{
			args.push_back(pc.words[0]);
			args.push_back(pc.words[1]);
			server->sendString(sender, format(ERR_USERONCHANNEL, args));
		}
		else
		{
			args.push_back(pc.words[0]);
			args.push_back(pc.words[1]);
			server->sendString(sender, format(RPL_INVITING, args));
			args.clear();
			args.push_back(sender->getNick());
			args.push_back(sender->getUser());
			args.push_back(sender->getHost());
			args.push_back(pc.words[0]);
			args.push_back(pc.words[1]);
			server->sendString(cli, format(RPL_INVITE, args));
			chan->addInvite(cli);
		}
	}
}