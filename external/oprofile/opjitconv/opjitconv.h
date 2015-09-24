/**
 * @file opjitconv.h
 * Convert a jit dump file to an ELF file
 *
 * @remark Copyright 2007 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Jens Wilke
 * @Modifications Maynard Johnson
 * @Modifications Philippe Elie
 * @Modifications Daniel Hansel
 *
 * Copyright IBM Corporation 2007
 *
 */

#ifndef OPJITCONV_H
#define OPJITCONV_H

#define OP_JIT_CONV_OK 0
#define OP_JIT_CONV_FAIL -1
#define OP_JIT_CONV_NO_DUMPFILE 1
#define OP_JIT_CONV_NO_ANON_SAMPLES 2
#define OP_JIT_CONV_NO_MATCHING_ANON_SAMPLES 3
#define OP_JIT_CONV_NO_JIT_RECS_IN_DUMPFILE 4
#define OP_JIT_CONV_ALREADY_DONE 5
#define OP_JIT_CONV_TMPDIR_NOT_REMOVED 6

#include <bfd.h>
#include <stddef.h>
#include <sys/stat.h>

#include "op_list.h"
#include "op_types.h"

struct jitentry {
	
	struct jitentry * next;
	
	unsigned long long vma;
	
	void const * code;
	
	int code_size;
	char * symbol_name;
	
	int sym_name_malloced;
	
	unsigned long long life_start;
	/* seconds since epoch when the code was overwritten */
	unsigned long long life_end;
	asection * section;
};

struct jitentry_debug_line {
	struct jitentry_debug_line * next;
	struct jr_code_debug_info const * data;
	
	unsigned long long life_start;
	/* seconds since epoch when the code was overwritten */
	unsigned long long life_end;
	void const * end;
};

struct op_jitdump_info
{
	void * dmp_file;
	struct stat dmp_file_stat;
};

struct pathname
{
	char * name;
	struct list_head neighbor;
};

void create_arrays(void);
int resolve_overlaps(unsigned long long start_time);
void disambiguate_symbol_names(void);

int parse_all(void const * start, void const * end,
	      unsigned long long end_time);

int op_jit_convert(struct op_jitdump_info file_info, char const * elffile,
                   unsigned long long start_time, unsigned long long end_time);

bfd * open_elf(char const * filename);
int partition_sections(void);
int fill_sections(void);
asection * create_section(bfd * abfd, char const * section_name,
			  size_t size, bfd_vma vma, flagword flags);
int fill_section_content(bfd * abfd, asection * section,
			 void const * b, file_ptr offset, size_t sz);

int init_debug_line_info(bfd * abfd);
int finalize_debug_line_info(bfd * abfd);

extern enum bfd_architecture dump_bfd_arch;
extern int dump_bfd_mach;
extern char const * dump_bfd_target_name;
extern struct jitentry * jitentry_list;
extern u32 entry_count;
extern struct jitentry_debug_line * jitentry_debug_line_list;
extern u32 max_entry_count;
extern struct jitentry ** entries_symbols_ascending;
extern struct jitentry ** entries_address_ascending;
extern asymbol ** syms;
extern bfd * cur_bfd;
extern int debug;


#endif 
