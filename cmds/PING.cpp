/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PING.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:01:33 by sleon             #+#    #+#             */
/*   Updated: 2023/09/16 17:35:03 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

void RFC1459_PING(Client* sender, parsedCmd_t& pcmd)
{
	if (!ensureAuth(sender))
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
    REGISTER_COMMAND("PING", sender);
    if (!pcmd.has_words)
        server->sendString(sender, format(ERR_NOORIGIN, args));
    else
    {
        for (size_t i = 0; i < pcmd.words.size(); ++i)
        {
            if (pcmd.words[i].compare(SERVER_NAME))
                RFC1459_ERR_NOSUCHSERVER(sender, pcmd.words[i]);
            else
            {
                args.clear();
                args.push_back(SERVER_NAME);
                args.push_back(SERVER_NAME);
                args.push_back(SERVER_NAME);
                server->sendString(sender, format(RPL_PING, args));
                cout << "PING/PONG" << endl;
            }
        }
    }
}
