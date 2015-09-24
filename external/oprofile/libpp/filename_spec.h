/**
 * @file filename_spec.h
 * Container holding a sample filename split into its components
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#ifndef FILENAME_SPEC_H
#define FILENAME_SPEC_H

#include <unistd.h>
#include <string>

#include "generic_spec.h"

class profile_spec;
class extra_images;

class filename_spec
{
	friend class profile_spec;

public:
	filename_spec(std::string const & filename,
		      extra_images const & extra);

	filename_spec();

	void set_sample_filename(std::string const & filename,
				 extra_images const & extra);

	bool match(filename_spec const & rhs,
	           std::string const & binary) const;

	bool is_dependent() const;

private:
	std::string image;
	std::string lib_image;
	std::string cg_image;
	std::string event;
	int count;
	unsigned int unitmask;
	generic_spec<pid_t> tgid;
	generic_spec<pid_t> tid;
	generic_spec<int> cpu;
};


#endif 
