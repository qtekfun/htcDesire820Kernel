/**
 * @file generic_spec.h
 * Container holding an item or a special "match all" item
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef GENERIC_SPEC_H
#define GENERIC_SPEC_H

#include <stdexcept>
#include <string>
#include <sstream>

#include "string_manip.h"


template <class T>
class generic_spec
{
public:
	generic_spec();

	
	
	
	void set(std::string const &);

	
	bool is_set() const {
		return !is_all;
	}

	
	T const value() const {
		if (!is_all)
			return data;
		throw std::out_of_range("generic_spec holds no value");
	}

	
	bool match(T const & rhs) const {
		return rhs == data;
	}

	
	bool match(generic_spec<T> const & rhs) const {
		return is_all || rhs.is_all || rhs.data == data;
	}

private:
	T data;
	bool is_all;
};


template <class T>
generic_spec<T>::generic_spec()
	:
	data(T()),
	is_all(true)
{
}


template <class T>
void generic_spec<T>::set(std::string const & str)
{
	if (str == "all") {
		is_all = true;
		return;
	}

	is_all = false;
	data = op_lexical_cast<T>(str);
}


template <>
void generic_spec<std::string>::set(std::string const & str);

#endif 
