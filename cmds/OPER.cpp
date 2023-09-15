/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:06:00 by sleon             #+#    #+#             */
/*   Updated: 2023/09/15 18:29:07 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

void	RFC1459_OPER(Client *sender, parsedCmd_t &pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("OPER", sender);
	Client	*client1;

	if (!pcmd.has_words || pcmd.words.size() != 2)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else if (pcmd.words[1].compare(server->getPassword()))
		RFC1459_ERR_PASSWDMISMATCH(sender);
	else
	{
		client1 = server->findClient(pcmd.words[0]);
		if (client1)
			RFC1459_ERR_NOSUCHNICK(sender, pcmd.words[0]);
		else
		{
			client1->getMode()->o = true;
			server->sendString(sender, format(RPL_YOUREOPER, args));
		}
	}
}
