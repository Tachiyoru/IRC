/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MOTD.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:26:45 by sleon             #+#    #+#             */
/*   Updated: 2023/09/22 16:27:21 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"
#include <fstream>

std::string	readMOTD(void) {

	std::ifstream	infile;
	std::string		motd;
	std::string		line;
	std::string		format = ":ircserv 372 archi :-";


	infile.open("bob_rc.motd");
	if (!infile.is_open())
		return ("");
	else {

		while (getline(infile, line)) {
			motd.append(format);
			motd.append(line);
			motd.append("\n");
		}
		return (motd);
	}
}


void RFC1459_MOTD(Client *sender, parsedCmd_t& pc) {

	std::string motd = readMOTD();

	if (motd.empty())
		return;

	REGISTER_COMMAND("MOTD", sender);

	if (!sender->hasAuthd() || !pc.has_words) {

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	args.push_back(SERVER_NAME);
	server->sendString(sender, format(RPL_MOTDSTART, args));

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	args.push_back(motd);
	server->sendString(sender, format(RPL_MOTD, args));

	REFRESH_REPLY(sender);
	args.clear();
	args.push_back(SERVER_NAME);
	args.push_back(sender->getNick());
	server->sendString(sender, format(RPL_ENDOFMOTD, args));

	}

	return ;
}
