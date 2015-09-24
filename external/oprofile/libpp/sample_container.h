/**
 * @file sample_container.h
 * Internal implementation of sample container
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef SAMPLE_CONTAINER_H
#define SAMPLE_CONTAINER_H

#include <map>
#include <set>
#include <string>

#include "symbol.h"
#include "symbol_functors.h"

class sample_container {
	typedef std::pair<symbol_entry const *, bfd_vma> sample_index_t;
public:
	typedef std::map<sample_index_t, sample_entry> samples_storage;
	typedef samples_storage::const_iterator samples_iterator;

	
	samples_iterator begin(symbol_entry const *) const;
	
	samples_iterator end(symbol_entry const *) const;

	
	samples_iterator begin() const;
	
	samples_iterator end() const;

	
	
	void insert(symbol_entry const * symbol, sample_entry const &);

	
	count_array_t accumulate_samples(debug_name_id filename_id) const;

	
	count_array_t accumulate_samples(debug_name_id, size_t linenr) const;

	
	sample_entry const * find_by_vma(symbol_entry const * symbol,
					 bfd_vma vma) const;

private:
	
	void build_by_loc() const;

	
	samples_storage samples;

	typedef std::multiset<sample_entry const *, less_by_file_loc>
		samples_by_loc_t;

	
	

	mutable samples_by_loc_t samples_by_loc;
};

#endif 
