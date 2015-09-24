/**
 * @file symbol_container.h
 * Internal container for symbols
 *
 * @remark Copyright 2002, 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef SYMBOL_CONTAINER_H
#define SYMBOL_CONTAINER_H

#include <string>
#include <set>

#include "symbol.h"
#include "symbol_functors.h"

class symbol_container {
public:
	
	typedef std::set<symbol_entry, less_symbol> symbols_t;

	typedef symbols_t::size_type size_type;

	
	size_type size() const;

	symbol_entry const * insert(symbol_entry const &);

	
	symbol_collection const find(debug_name_id filename, size_t linenr) const;

	
	symbol_collection const find(debug_name_id filename) const;

	
	symbol_entry const * find_by_vma(std::string const & image_name,
					 bfd_vma vma) const;

	
	symbol_entry const * find(symbol_entry const & symbol) const;

	
	symbols_t::iterator begin();

	
	symbols_t::iterator end();

private:
	
	void build_by_loc() const;

	symbols_t symbols;

	typedef std::multiset<symbol_entry const *, less_by_file_loc>
		symbols_by_loc_t;

	

	mutable symbols_by_loc_t symbols_by_loc;
};

#endif 
