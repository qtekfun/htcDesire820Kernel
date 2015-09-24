/**
 * @file common_option.h
 * Declaration of entry point of pp tools, implementation file add common
 * options of pp tools and some miscelleaneous functions
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef COMMON_OPTION_H
#define COMMON_OPTION_H

#include <vector>
#include <list>

#include "arrange_profiles.h"
#include "demangle_symbol.h"

namespace options {
	extern bool verbose;
	extern double threshold;
	extern std::string threshold_opt;
	extern std::string command_options;
	extern std::vector<std::string> image_path;
	extern std::string root_path;

	struct spec {
		std::list<std::string> common;
		std::list<std::string> first;
		std::list<std::string> second;
	};
}

typedef int (*pp_fct_run_t)(options::spec const & spec);

int run_pp_tool(int argc, char const * argv[], pp_fct_run_t fct);

demangle_type handle_demangle_option(std::string const & option);

merge_option handle_merge_option(std::vector<std::string> const & mergespec,
       bool allow_lib, bool exclude_dependent);

#endif 
