/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleon <sleon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 16:14:53 by adegain           #+#    #+#             */
/*   Updated: 2023/09/22 14:04:28 by sleon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <string>
#include <strings.h>
#include <iostream>
#include "RFC1459.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>


using namespace std;

typedef struct parsedCmd_t
{
	string			cmd;
	bool			has_words;
	vector<string>	words;
	bool			has_suffix;
	string			suffix;
}parsedCmd_t;

typedef struct parsedMode_s
{
	bool	valid;
	string	invalidMode;
	bool	sign;
	string	modes;
}parsedMode_t;

vector<string>	split(const string& s, const string& delimiter);
parsedCmd_t		parse(const string& s);
string			format(const string& s, const vector<std::string>& values);
string			getDateAndTime(bool inc_date, bool inc_time);
parsedMode_t	parse_mode(string s, bool for_client);

#endif
