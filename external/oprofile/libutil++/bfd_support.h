/**
 * @file bfd_support.h
 * BFD muck we have to deal with.
 *
 * @remark Copyright 2005 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef BFD_SUPPORT_H
#define BFD_SUPPORT_H

#include "utility.h"
#include "op_types.h"
#include "locate_images.h"

#include <bfd.h>
#include <stdint.h>

#include <string>

class op_bfd_symbol;

struct bfd_info {
	bfd_info() : abfd(0), nr_syms(0), synth_syms(0), image_bfd_info(0) {}

	~bfd_info();

	
	void close();

	
	bool valid() const { return abfd; }

	
	bool has_debug_info() const;

	
	void get_symbols();

	
	bfd * abfd;
	
	scoped_array<asymbol *> syms;
	
	size_t nr_syms;

	void set_image_bfd_info(bfd_info * ibfd) { image_bfd_info = ibfd; }

private:
	bool get_synth_symbols();

	asymbol * synth_syms;

 
	bfd_info * image_bfd_info;

	void translate_debuginfo_syms(asymbol ** dbg_syms, long nr_dbg_syms);

};


extern bool
find_separate_debug_file(bfd * ibfd, 
                         std::string const & filepath_in,
                         std::string & debug_filename,
                         extra_images const & extra);

bfd * open_bfd(std::string const & file);

bfd * fdopen_bfd(std::string const & file, int fd);

bfd * spu_open_bfd(std::string const name, int fd, uint64_t offset_to_spu_elf);

bool interesting_symbol(asymbol * sym);

bool boring_symbol(op_bfd_symbol const & first, op_bfd_symbol const & second);

struct linenr_info {
	
	bool found;
	
	std::string filename;
	
	unsigned int line;
};

linenr_info const
find_nearest_line(bfd_info const & ibfd, op_bfd_symbol const & sym,
                  bfd_vma offset, bool anon_obj);

#endif 
