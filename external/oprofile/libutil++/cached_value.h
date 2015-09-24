/**
 * @file cached_value.h
 * Hold a cached value.
 *
 * @remark Copyright 2005 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef CACHED_VALUE_H
#define CACHED_VALUE_H

#include "op_exception.h"

template <class T>
class cached_value
{
public:
	cached_value() : set(false) {}

	typedef T value_type;

	
	value_type const get() const {
		if (!set)
			throw op_fatal_error("cached value not set");
		return value;
	}

	
	bool cached() const { return set; }

	
	value_type const reset(value_type const & val) {
		value = val;
		set = true;
		return value;
	}

private:
	
	value_type value;
	
	bool set;
};


#endif 
