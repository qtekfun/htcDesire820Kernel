/**
 * @file demangle_symbol.h
 * Demangle a C++ symbol
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef DEMANGLE_SYMBOL_H
#define DEMANGLE_SYMBOL_H

#include <string>

enum demangle_type {
	
	dmt_none,
	
	dmt_normal,
	
	
	dmt_smart
};

std::string const demangle_symbol(std::string const & name);

#endif 
