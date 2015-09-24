/**
 * @file profile_container.h
 * Container associating symbols and samples
 *
 * @remark Copyright 2002, 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef PROFILE_CONTAINER_H
#define PROFILE_CONTAINER_H

#include <string>
#include <vector>

#include "profile.h"
#include "utility.h"
#include "op_bfd.h"
#include "sample_container.h"
#include "symbol_container.h"
#include "format_flags.h"
#include "locate_images.h"

class string_filter;
class symbol_entry;
class sample_entry;

class profile_container : noncopyable {
public:
	profile_container(bool debug_info, bool need_details,
			  extra_images const & extra);

	~profile_container();
 
	void add(profile_t const & profile, op_bfd const & abfd,
		 std::string const & app_name, size_t pclass);

	
	
	symbol_entry const * find_symbol(std::string const & image_name,
					 bfd_vma vma) const;

	
	
	symbol_collection const find_symbol(debug_name_id filename,
					size_t linenr) const;

	
	
	sample_entry const * find_sample(symbol_entry const * symbol,
					 bfd_vma vma) const;

	
	symbol_entry const * find(symbol_entry const & symbol) const;

	
	struct symbol_choice {
		symbol_choice()
			: hints(cf_none), threshold(0.0), match_image(false) {}

		
		column_flags hints;
		
		double threshold;
		
		bool match_image;
		
		std::string image_name;
	};

	symbol_collection const select_symbols(symbol_choice & choice) const;

	symbol_collection const select_symbols(debug_name_id filename) const;

	
	std::vector<debug_name_id> const select_filename(double threshold) const;

	
	count_array_t samples_count() const;

	
	
	count_array_t samples_count(debug_name_id filename_id) const;
	
	
	count_array_t samples_count(debug_name_id filename,
			   size_t linenr) const;

	
	symbol_container::symbols_t::iterator begin_symbol() const;
	
	symbol_container::symbols_t::iterator end_symbol() const;

	
	sample_container::samples_iterator begin() const;
	
	sample_container::samples_iterator end() const;

	
	sample_container::samples_iterator begin(symbol_entry const *) const;
	
	sample_container::samples_iterator end(symbol_entry const *) const;

private:
	
	void add_samples(op_bfd const & abfd, symbol_index_t sym_index,
	                 profile_t::iterator_pair const &,
	                 symbol_entry const * symbol, size_t pclass,
			 unsigned long start);

	std::string create_artificial_symbol(op_bfd const & abfd, u32 start,
	                                     u32 & end, size_t & order);

	
	
	scoped_ptr<symbol_container> symbols;
	
	
	scoped_ptr<sample_container> samples;
	
	
	count_array_t total_count;

	
	bool debug_info;
	bool need_details;
	

public: 
	extra_images extra_found_images;
};

#endif 
