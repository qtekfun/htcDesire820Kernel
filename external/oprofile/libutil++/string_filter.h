/**
 * @file string_filter.h
 * Filter strings based on exclude/include list
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef STRING_FILTER_H
#define STRING_FILTER_H

#include <string>
#include <vector>

class string_filter {
public:
	string_filter() {}

	string_filter(std::string const & include_patterns,
	              std::string const & exclude_patterns);

	string_filter(std::vector<std::string> const & include_patterns,
	              std::vector<std::string> const & exclude_patterns);

	virtual ~string_filter() {}

	
	virtual bool match(std::string const & str) const;

protected:
	
	std::vector<std::string> include;
	
	std::vector<std::string> exclude;
};

#endif 
