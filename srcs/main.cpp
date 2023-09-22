/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 13:39:25 by adegain           #+#    #+#             */
/*   Updated: 2023/09/22 17:36:03 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include "Server.hpp"
#include <signal.h>

static void sighandler(int sig)
{
	if (sig == SIGINT)
		server->setStop(true);
}

int main(int ac, const char* const av[])
{
	size_t cpy, i;
	pollfd* pfd;

	if (ac != 3)
	{
		cerr << "usage: ./ircserv <port> <password>" << endl;
		return 1;
	}
	int value = strtol(av[1], NULL, 10);
	if (value < 1024 || value > 65535) {
		std::cout << "ERROR: INVALID PORT VALUE" << std::endl;
		return 2;
	}
	server = new Server((uint16_t)atoi(av[1]), av[2]);

	if (!server->start())
	{
		cerr << SERVER_NAME << ": could not start server" << endl;
		delete server;
		return 3;
	}
	else
	{
		cout << SERVER_NAME << ": server started" << endl;
		signal(SIGINT, &sighandler);
		while (!server->getStop())
		{
			cpy = server->getSize();

			if (poll(server->getPfd(0), cpy, 0) == -1)
				break;

			for (i = 0; i < cpy; ++i)
			{
				pfd = server->getPfd((int)i);

				if (!pfd || pfd->fd == -1 || pfd->revents != POLLIN)
					continue;
				if (pfd->fd == server->getSocket())
					server->acceptClient();
				else
				{
					server->handleRequest(pfd);
				}
			}
		}
		cout << SERVER_NAME << ": server stopped" << endl;
	}
	delete server;
	return 0;
}
