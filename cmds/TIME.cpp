/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 22:14:26 by sleon             #+#    #+#             */
/*   Updated: 2023/09/18 18:38:59 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	RFC1459_TIME(Client *sender, parsedCmd_t pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("TIME", sender);
	if (!pcmd.has_words || !pcmd.words[0].compare(SERVER_NAME))
	{
		args.push_back(SERVER_NAME);
		args.push_back(getDateAndTime(1, 1));
		server->sendString(sender, format(RPL_TIME, args));
	}
	else
		RFC1459_ERR_NOSUCHSERVER(sender, pcmd.words[0]);
}
