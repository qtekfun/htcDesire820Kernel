/**
 * @file op_regex.h
 * This file contains various definitions and interface for a
 * lightweight wrapper around libc regex, providing match
 * and replace facility.
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 * @remark Idea comes from TextFilt project <http://textfilt.sourceforge.net>
 *
 * @author Philippe Elie
 */

#ifndef OP_REGEX_H
#define OP_REGEX_H

#include <sys/types.h>
#include <regex.h>

#include <string>
#include <vector>
#include <map>

#include "op_exception.h"

struct bad_regex : op_exception {
	bad_regex(std::string const & pattern);
};

class regular_expression_replace {
public:
	regular_expression_replace(size_t limit = 100,
				   size_t limit_defs_expansion = 100);
	~regular_expression_replace();

	void add_definition(std::string const & name,
			    std::string const & replace);
	void add_pattern(std::string const & pattern,
			 std::string const & replace);

	bool execute(std::string & str) const;
private:
	struct replace_t {
		
		regex_t regexp;
		
		std::string replace;
	};

	
	bool do_execute(std::string & str, replace_t const & regexp) const;
	void do_replace(std::string & str, std::string const & replace,
			regmatch_t const * match) const;

	
	std::string expand_string(std::string const & input);

	
	std::string substitute_definition(std::string const & pattern);

	
	regmatch_t const & get_match(regmatch_t const * match, char idx) const;

	
	
	
	
	
	static const size_t max_match = 16;

	size_t limit;
	size_t limit_defs_expansion;
	std::vector<replace_t> regex_replace;
	
	typedef std::map<std::string, std::string> defs_dict;
	defs_dict defs;
};

void setup_regex(regular_expression_replace& regex,
		 std::string const & filename);

#endif 
