/**
 * @file string_manip.h
 * std::string helpers
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef STRING_MANIP_H
#define STRING_MANIP_H

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

std::string erase_to_last_of(std::string const & str, char ch);

std::string split(std::string & s, char c);

bool is_prefix(std::string const & s, std::string const & prefix);

std::vector<std::string> separate_token(std::string const & str, char sep);

std::string ltrim(std::string const & str, std::string const & totrim = "\t ");
std::string rtrim(std::string const & str, std::string const & totrim = "\t ");
std::string trim(std::string const & str, std::string const & totrim = "\t ");

std::string const
format_percent(double value, size_t int_width,
               size_t frac_width, bool showpos = false);

static unsigned int const percent_int_width = 2;
static unsigned int const percent_fract_width = 4;
static unsigned int const percent_width = percent_int_width + percent_fract_width + 1;


template <typename To, typename From>
To op_lexical_cast(From const & src)
{
	std::ostringstream in;
	if (!(in << src))
		throw std::invalid_argument("op_lexical_cast<T>()");
	std::istringstream out(in.str());
	To value;
	if (!(out >> value)) {
		throw std::invalid_argument("op_lexical_cast<T>(\"" +
		    in.str() +"\")");
	}
	return value;
}

template <>
unsigned int op_lexical_cast<unsigned int>(std::string const & str);

#endif 
