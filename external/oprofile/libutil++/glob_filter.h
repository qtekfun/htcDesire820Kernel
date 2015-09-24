/**
 * @file glob_filter.h
 * Filter strings based on globbed exclude/include list
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef GLOB_FILTER_H
#define GLOB_FILTER_H

#include "string_filter.h"

class glob_filter : public string_filter {
public:
	glob_filter(std::string const & include_patterns,
	            std::string const & exclude_patterns)
		: string_filter(include_patterns, exclude_patterns) {}

	glob_filter(std::vector<std::string> const & include_patterns,
	            std::vector<std::string> const & exclude_patterns)
		: string_filter(include_patterns, exclude_patterns) {}

	
	virtual bool match(std::string const & str) const;

protected:

	
	struct fnmatcher {
		fnmatcher(std::string const & str) : str_(str) {}

		bool operator()(std::string const & s);

		std::string const & str_;
	};
};

#endif 
