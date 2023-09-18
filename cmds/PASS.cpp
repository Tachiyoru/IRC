/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:30:27 by sleon             #+#    #+#             */
/*   Updated: 2023/09/16 17:34:03 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RFC1459.hpp"
#include "Server.hpp"

void	RFC1459_PASS(Client *sender, parsedCmd_t &pcmd)
{
	REGISTER_COMMAND("PASS", sender);
	if(sender->hasPwd())
		RFC1459_ERR_ALREADYREGISTRED(sender);
	else if (!pcmd.has_words)
		RFC1459_ERR_NEEDMOREPARAMS(sender, cmd_name);
	else if (pcmd.words[0]!= server->getPassword())
		RFC1459_ERR_PASSWDMISMATCH(sender);
	else
		sender->hasPwd(true);
}
