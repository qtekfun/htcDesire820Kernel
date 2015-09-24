/**
 * @file diff_container.h
 * Container for diffed symbols
 *
 * @remark Copyright 2005 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef DIFF_CONTAINER_H
#define DIFF_CONTAINER_H

#include "profile_container.h"


class diff_container : noncopyable {
public:
	
	diff_container(profile_container const & pc1,
	               profile_container const & pc2);

	~diff_container() {}
 
	
	diff_collection const
		get_symbols(profile_container::symbol_choice & choice) const;

	
	count_array_t const samples_count() const;

private:
	
	profile_container const & pc1;

	
	profile_container const & pc2;

	
	count_array_t total1;

	
	count_array_t total2;
};

#endif 
