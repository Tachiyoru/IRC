/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:45:36 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 17:53:45 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

void	RFC1459_QUIT(Client *sender, parsedCmd_t &pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("QUIT", sender);
	std::string						reason = "Client Quit";
	map<string, Channel*>			*chan;
	map<string, Channel*>::iterator	it;
	if (pcmd.has_suffix)
		reason = pcmd.suffix;
	args.clear();
	args.push_back(sender->getNick());
	args.push_back(sender->getUser());
	args.push_back(sender->getHost());
	args.push_back(reason);
	server->sendString(sender, format(RPL_QUIT, args));
	chan = server->getChannelList();
	it = chan->begin();
	for (; it != chan->end(); ++it)
	{
		if (it->second->onChannel(sender->getFd()))
			it->second->broadcast(format(RPL_QUIT, args), sender);
	}
	server->disconnectClient(sender->getPfd());
}
