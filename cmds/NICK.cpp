/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 15:10:07 by sleon             #+#    #+#             */
/*   Updated: 2023/09/16 16:55:12 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

static bool	is_valid_nick(const string &s)
{
	return s.find_first_of(RFC1459_FORBIDDEN) == std::string::npos;
}

static bool	fix_invalid_nick(string &s)
{
	size_t pos = 0;
	size_t cpy = s.size();

	for (size_t i = 0; i < cpy; i++)
	{
		pos = s.find_first_of(RFC1459_FORBIDDEN);
		if (pos != std::string::npos)
			s.erase(pos, 1);
	}
	return (s.empty() == true) ? false : true;
}

void 	RFC1459_NICK(Client *sender, parsedCmd_t pcmd)
{
	if (!sender->hasPwd())
	{
		RFC1459_ERR_NOTREGISTERED(sender);
		return;
	}
	REGISTER_COMMAND("NICK", sender);
	std::string	newNick;
	if (!pcmd.has_words)
	{
		RFC1459_ERR_NONICKNAMEGIVEN(sender);
		return;
	}
	else if (!is_valid_nick(pcmd.words[0]))
	{
		args.push_back(pcmd.words[0]);
		server->sendString(sender, format(ERR_ERRONEUSNICKNAME, args));
		if (!fix_invalid_nick(pcmd.words[0]))
			goto swapRandNick;
	}
	if (server->findClient(pcmd.words[0]))
	{
		REFRESH_REPLY(sender);
		args.push_back(pcmd.words[0]);
		server->sendString(sender, format(ERR_NICKNAMEINUSE, args));
		if (!sender->getUser().compare("*"))
		{
swapRandNick:
			newNick = pcmd.words[0] + "_";
			while (server->findClient(newNick))
				newNick += "_";
			pcmd.words[0] = newNick;
		}
		else
			return;
	}
	if (sender->getUser().compare("*"))
	{
		args.clear();
		args.push_back(sender->getNick());
		args.push_back(sender->getUser());
		args.push_back(sender->getHost());
		args.push_back(pcmd.words[0]);
		server->sendString(sender, format(RPL_NICK, args));
	}
	sender->setNick(pcmd.words[0]);
	std::cout<<"nick has been set to "<<pcmd.words[0]<<std::endl;
}
