/**
 * @file op_bfd.h
 * Encapsulation of bfd objects
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef OP_BFD_H
#define OP_BFD_H

#include "config.h"

#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>

#include "bfd_support.h"
#include "locate_images.h"
#include "utility.h"
#include "cached_value.h"
#include "op_types.h"

class op_bfd;
class string_filter;
class extra_images;

typedef size_t symbol_index_t;

class op_bfd_symbol {
public:

	
	op_bfd_symbol(asymbol const * a);

	
	op_bfd_symbol(bfd_vma vma, size_t size, std::string const & name);

	bfd_vma vma() const { return symb_value + section_vma; }
	unsigned long value() const { return symb_value; }
	unsigned long filepos() const { return symb_value + section_filepos; }
	unsigned long symbol_endpos(void) const;
	asection const * section(void) const { return bfd_symbol->section; }
	std::string const & name() const { return symb_name; }
	asymbol const * symbol() const { return bfd_symbol; }
	size_t size() const { return symb_size; }
	void size(size_t s) { symb_size = s; }
	bool hidden() const { return symb_hidden; }
	bool weak() const { return symb_weak; }
	bool artificial() const { return symb_artificial; }

	
	bool operator<(op_bfd_symbol const & lhs) const;

private:
	
	
	asymbol const * bfd_symbol;
	
	
	unsigned long symb_value;
	
	unsigned long section_filepos;
	
	bfd_vma section_vma;
	
	size_t symb_size;
	
	std::string symb_name;
	
	bool symb_hidden;
	
	bool symb_weak;
	
	bool symb_artificial;
	
	std::string symb_bytes;
};

class op_bfd {
public:
	op_bfd(std::string const & filename,
	       string_filter const & symbol_filter,
	       extra_images const & extra_images,
	       bool & ok);

	op_bfd(uint64_t spu_offset,
	       std::string const & filename,
	       string_filter const & symbol_filter,
	       extra_images const & extra_images,
	       bool & ok);

	std::string get_embedding_filename() const { return embedding_filename; }

	
	~op_bfd();

	bool get_linenr(symbol_index_t sym_idx, bfd_vma offset,
			std::string & filename, unsigned int & linenr) const;

	void get_symbol_range(symbol_index_t sym_idx,
			      unsigned long long & start, unsigned long long & end) const;

	void get_vma_range(bfd_vma & start, bfd_vma & end) const;

	
	bfd_vma offset_to_pc(bfd_vma offset) const;

	unsigned long get_start_offset(bfd_vma vma = 0) const;
 
	std::string get_filename() const;

	
	std::vector<op_bfd_symbol> syms;

	
	
	
	size_t bfd_arch_bits_per_address() const;

	
	bool has_debug_info() const;

	bool symbol_has_contents(symbol_index_t sym_idx);

	bool get_symbol_contents(symbol_index_t sym_index,
		unsigned char * contents) const;

	bool valid() const { return ibfd.valid(); }

private:
	
	typedef std::list<op_bfd_symbol> symbols_found_t;

	void get_symbols(symbols_found_t & symbols);

	void get_symbols_from_file(bfd_info & bfd, size_t start,
				   op_bfd::symbols_found_t & symbols,
				   bool debug_file);

	void add_symbols(symbols_found_t & symbols,
	                 string_filter const & symbol_filter);

	size_t symbol_size(op_bfd_symbol const & sym,
			   op_bfd_symbol const * next) const;

	
	op_bfd_symbol const create_artificial_symbol();

	uint process_symtab(bfd_info * bfd, uint start);

	
	std::string filename;

	
	std::string archive_path;

	
	extra_images const & extra_found_images;

	
	off_t file_size;

	
	mutable std::string debug_filename;

	
	mutable cached_value<bool> debug_info;

	
	bfd_info ibfd;

	
	mutable bfd_info dbfd;

	
	
	
	
	std::vector<asection const *> filtered_section;

	typedef std::map<std::string, u32> filepos_map_t;
	
	filepos_map_t filepos_map;

	std::string embedding_filename;

	bool anon_obj;
};


#endif 
