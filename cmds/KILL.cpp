/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KILL.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:53:46 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 18:38:35 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void RFC1459_KILL(Client *sender, parsedCmd_t pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("KILL", sender);
	Client	*reciever;

	if (!pcmd.has_words || pcmd.has_suffix)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else
	{
		if (!sender->getMode() -> o)
		{
			server->sendString(sender, format(ERR_NOPRIVILEGES, args));
			return;
		}
		reciever = server->findClient(pcmd.words[0]);

		if (!reciever)
			RFC1459_ERR_NOSUCHNICK(sender, pcmd.words[0]);
		else
		{
			args.push_back(pcmd.suffix);
			server->sendString(sender, format(RPL_KILL, args));
			server->disconnectClient(reciever->getPfd());
		}
	}
}
