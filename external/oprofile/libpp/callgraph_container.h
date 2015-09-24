/**
 * @file callgraph_container.h
 * Container associating symbols and caller/caller symbols
 *
 * @remark Copyright 2004 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef CALLGRAPH_CONTAINER_H
#define CALLGRAPH_CONTAINER_H

#include <set>
#include <vector>
#include <string>

#include "symbol.h"
#include "symbol_functors.h"
#include "string_filter.h"
#include "locate_images.h"

class profile_container;
class inverted_profile;
class profile_t;
class image_set;
class op_bfd;


class arc_recorder {
public:
	~arc_recorder() {}

	void add(symbol_entry const & caller, symbol_entry const * callee,
	         count_array_t const & arc_count);

	
	symbol_collection const & get_symbols() const;

	void process(count_array_t total, double threshold,
	             string_filter const & filter);

private:
	struct cg_data {
		cg_data() {}

		typedef std::map<symbol_entry, count_array_t, less_symbol> children;

		
		children callers;
		
		children callees;
	};

	void process_children(cg_symbol & sym, double threshold);

	typedef std::map<symbol_entry, cg_data, less_symbol> map_t;

	
	map_t sym_map;

	
	cg_collection_objs cg_syms_objs;

	
	symbol_collection cg_syms;
};


class callgraph_container {
public:
	void populate(std::list<inverted_profile> const & iprofiles,
		      extra_images const & extra, bool debug_info,
		      double threshold, bool merge_lib,
		      string_filter const & sym_filter);

	
	column_flags output_hint() const;

	
	count_array_t samples_count() const;

	
	symbol_collection const & get_symbols() const;

private:
	void add(profile_t const & profile, op_bfd const & caller_bfd,
	         bool bfd_caller_ok, op_bfd const & callee_bfd,
		 std::string const & app_name, profile_container const & pc,
		 bool debug_info, size_t pclass);

	void populate(std::list<image_set> const & lset,
		      std::string const & app_image,
		      size_t pclass, profile_container const & pc,
		      bool debug_info, bool merge_lib);
	void populate(std::list<std::string> const & cg_files,
		      std::string const & app_image,
		      size_t pclass, profile_container const & pc,
		      bool debug_info, bool merge_lib);

	
	void add_symbols(profile_container const & pc);

	
	count_array_t total_count;

	
	arc_recorder recorder;

public:  
	extra_images extra_found_images;
};

#endif 
