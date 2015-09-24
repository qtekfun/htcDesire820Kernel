/**
 * @file opgprof_options.h
 * Options for opgprof tool
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPGPROF_OPTIONS_H
#define OPGPROF_OPTIONS_H

#include <string>

#include "common_option.h"

namespace options {
	extern std::string gmon_filename;
}

class inverted_profile;

extern inverted_profile image_profile;

void handle_options(options::spec const & spec);

#endif 
