/**
 * @file symbol.h
 * Symbol containers
 *
 * @remark Copyright 2002, 2004 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "name_storage.h"
#include "growable_vector.h"
#include "sparse_array.h"
#include "format_flags.h"
#include "op_types.h"

#include <bfd.h>
#include <stdint.h>

#include <list>

class extra_images;


typedef sparse_array<u32, count_type> count_array_t;


struct file_location {
	file_location() : linenr(0) {}
	
	debug_name_id filename;
	
	unsigned int linenr;

	bool operator<(file_location const & rhs) const {
		
		return filename < rhs.filename ||
		  (filename == rhs.filename && linenr < rhs.linenr);
	}
};


struct sample_entry {
	sample_entry() : vma(0) {}
	
	file_location file_loc;
	
	bfd_vma vma;
	
	count_array_t counts;
};


class symbol_entry {
public:
	symbol_entry() : size(0) {}
	virtual ~symbol_entry() {}

	
	image_name_id image_name;
	
	
	
	image_name_id app_name;
	
	size_t sym_index;
	
	sample_entry sample;
	
	symbol_name_id name;
	
	size_t size;

	column_flags output_hint(column_flags fl) const;
	uint64_t spu_offset;
	image_name_id embedding_filename;
};


typedef std::vector<symbol_entry const *> symbol_collection;


class cg_symbol : public symbol_entry {
public:
	cg_symbol(symbol_entry const & sym) : symbol_entry(sym) {}

	typedef std::vector<symbol_entry> children;

	
	children callers;
	
	count_array_t total_caller_count;

	
	children callees;
	
	count_array_t total_callee_count;
};

typedef std::list<cg_symbol> cg_collection_objs;

typedef growable_vector<double> diff_array_t;


struct diff_symbol : public symbol_entry  {
	diff_symbol(symbol_entry const & sym) : symbol_entry(sym) {}

	
	diff_array_t diffs;
};


typedef std::vector<diff_symbol> diff_collection;

bool has_sample_counts(count_array_t const & counts, size_t lo, size_t hi);
std::string const & get_image_name(image_name_id id,
				   image_name_storage::image_name_type type,
				   extra_images const & extra);


#endif 
