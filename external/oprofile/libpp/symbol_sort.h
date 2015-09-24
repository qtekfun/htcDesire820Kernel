/**
 * @file symbol_sort.h
 * Sorting symbols
 *
 * @remark Copyright 2002, 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef SYMBOL_SORT_H
#define SYMBOL_SORT_H

#include "symbol.h"

#include <vector>
#include <string>

struct sort_options {
	enum sort_order {
		
		first,
		sample = first,
		image,
		app_name,
		symbol,
		debug,
		vma,
		last
	};

	sort_options() {}

	void add_sort_option(std::string const & name);
	void add_sort_option(sort_order order);

	void sort(symbol_collection & syms, bool reverse_sort,
	          bool long_filenames) const;

	void sort(diff_collection & syms, bool reverse_sort,
	          bool long_filenames) const;

	std::vector<sort_order> options;
};

#endif 
