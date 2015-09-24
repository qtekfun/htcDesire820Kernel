/**
 * @file format_flags.h
 * output options
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef FORMAT_FLAGS_H
#define FORMAT_FLAGS_H

enum format_flags {
	ff_none = 0,
	
	ff_vma = 1 << 0,
	
	ff_linenr_info = 1 << 1,
	
	ff_image_name = 1 << 3,
	
	ff_app_name = 1 << 4,
	
	ff_symb_name = 1 << 5,

	
	
	
	ff_nr_samples = 1 << 6,
	
	ff_nr_samples_cumulated = 1 << 7,
	
	ff_percent = 1 << 8,
	
	ff_percent_cumulated = 1 << 9,
	ff_percent_details = 1 << 10,
	ff_percent_cumulated_details = 1 << 11,
	
	ff_diff = 1 << 12,
	
};


enum column_flags {
	cf_none = 0,
	cf_64bit_vma = 1 << 0,
	cf_image_name = 1 << 1
};

#endif 
