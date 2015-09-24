/**
 * @file arrange_profiles.h
 * Classify and process a list of candidate sample files
 * into merged sets and classes.
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef ARRANGE_PROFILES_H
#define ARRANGE_PROFILES_H

#include <string>
#include <list>
#include <vector>
#include <iosfwd>

#include "image_errors.h"
#include "locate_images.h"

struct merge_option {
	bool cpu;
	bool lib;
	bool tid;
	bool tgid;
	bool unitmask;
};


struct profile_template {
	std::string event;
	std::string count;
	std::string unitmask;
	std::string tgid;
	std::string tid;
	std::string cpu;
};


struct profile_sample_files {
	std::string sample_filename;
	std::list<std::string> cg_files;
};


struct profile_dep_set {
	
	std::string lib_image;

	
	std::list<profile_sample_files> files;
};

struct profile_set {
	std::string image;

	
	
	std::list<profile_sample_files> files;

	
	std::list<profile_dep_set> deps;
};


struct profile_class {
	std::list<profile_set> profiles;

	
	std::string name;

	
	std::string longname;

	
	profile_template ptemplate;
};


enum axis_types {
	AXIS_EVENT,
	AXIS_TGID,
	AXIS_TID,
	AXIS_CPU,
	AXIS_MAX
};


struct profile_classes {
	std::string event;

	
	std::string cpuinfo;

	
	std::vector<profile_class> v;

	
	axis_types axis;

	
	extra_images extra_found_images;

	
	bool matches(profile_classes const & classes);
};


std::ostream & operator<<(std::ostream &, profile_sample_files const &);
std::ostream & operator<<(std::ostream &, profile_dep_set const &);
std::ostream & operator<<(std::ostream &, profile_set const &);
std::ostream & operator<<(std::ostream &, profile_template const &);
std::ostream & operator<<(std::ostream &, profile_class const &);
std::ostream & operator<<(std::ostream &, profile_classes const &);


profile_classes const
arrange_profiles(std::list<std::string> const & files,
		 merge_option const & merge_by, extra_images const & extra);


struct image_set {
	
	
	std::string app_image;

	
	std::list<profile_sample_files> files;
};

typedef std::list<image_set> image_group_set;

struct inverted_profile {
	inverted_profile() : error(image_ok) {}
	
	std::string image;

	
	mutable image_error error;

	
	std::vector<image_group_set> groups;
};


std::list<inverted_profile> const
invert_profiles(profile_classes const & classes);

#endif 
