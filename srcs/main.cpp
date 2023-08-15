/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:39:25 by adegain           #+#    #+#             */
/*   Updated: 2023/08/15 16:41:35 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>
#include <signal.h>

bool	checkPort(std::string str, int value)
{
	std::string::const_iterator	it;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it))
		{
			std::cerr<<"error: port: nondigit value"<<std::endl;
			return false;
		}
	}
	if ((value >= 0 && value < 1024) || value > 65535)
	{
		std::cerr<<"error: port: out of range"<<std::endl;
		return false;
	}
	return true;
}

void	sigHandler(int sig)
{
	if (sig == SIGINT){
		// server.stop();
		exit(4);
	}
}

int	main(int argc, char** argv)
{
	Server		server;

	if (argc != 3)
		return(std::cerr<<"Irc should run as : ./ircserv <port> <password>"<<std::endl, 1);
	if (!checkPort(argv[1], atoi(argv[1])))
		return 2;
	signal(SIGINT, sigHandler);
	if (server.init(atoi(argv[1]), argv[2]) || server.run())
		return 3;

}
