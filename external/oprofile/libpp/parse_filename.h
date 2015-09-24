/**
 * @file parse_filename.h
 * Split a sample filename into its constituent parts
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#ifndef PARSE_FILENAME_H
#define PARSE_FILENAME_H

#include <string>

class extra_images;

struct parsed_filename
{
	std::string image;
	std::string lib_image;
	
	
	std::string cg_image;
	std::string event;
	std::string count;
	std::string unitmask;
	std::string tgid;
	std::string tid;
	std::string cpu;

	
	bool profile_spec_equal(parsed_filename const & parsed);

	std::string filename;
	bool jit_dumpfile_exists;
};


std::ostream & operator<<(std::ostream &, parsed_filename const &);


parsed_filename parse_filename(std::string const & filename,
			       extra_images const & extra_found_images);

#endif 
