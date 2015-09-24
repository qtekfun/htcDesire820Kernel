/**
 * @file path_filter.h
 * Filter paths based on globbed exclude/include list
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef PATH_FILTER_H
#define PATH_FILTER_H

#include "glob_filter.h"

class path_filter : public glob_filter {
public:
	path_filter(std::string const & include_patterns = std::string(),
	            std::string const & exclude_patterns = std::string())
		: glob_filter(include_patterns, exclude_patterns) {}

	path_filter(std::vector<std::string> const & include_patterns,
	            std::vector<std::string> const & exclude_patterns)
		: glob_filter(include_patterns, exclude_patterns) {}


	
	virtual bool match(std::string const & str) const;
};

#endif 
