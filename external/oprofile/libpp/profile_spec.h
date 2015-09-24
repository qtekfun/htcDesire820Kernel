/**
 * @file profile_spec.h
 * Contains a PP profile specification
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#ifndef PROFILE_SPEC_H
#define PROFILE_SPEC_H

#include <map>
#include <vector>
#include <list>

#include "filename_spec.h"
#include "comma_list.h"
#include "locate_images.h"

class profile_spec
{
public:
	static profile_spec create(std::list<std::string> const & args,
	                           std::vector<std::string> const & image_path,
				   std::string const & root_path);

	std::list<std::string>
	generate_file_list(bool exclude_dependent, bool exclude_cg) const;

	bool match(filename_spec const & file_spec) const;

	std::string get_archive_path() const;

private:
	profile_spec();

	void parse(std::string const & tag_value);

	void set_image_or_lib_name(std::string const & image);

	bool is_valid_tag(std::string const & str);

	void parse_archive_path(std::string const &);
	void parse_session(std::string const &);
	void parse_session_exclude(std::string const &);
	void parse_image(std::string const &);
	void parse_image_exclude(std::string const &);
	void parse_lib_image(std::string const &);
	void parse_event(std::string const &);
	void parse_count(std::string const &);
	void parse_unitmask(std::string const &);
	void parse_tid(std::string const &);
	void parse_tgid(std::string const &);
	void parse_cpu(std::string const &);

	typedef void (profile_spec::*action_t)(std::string const &);
	typedef std::map<std::string, action_t> parse_table_t;
	parse_table_t parse_table;

	action_t get_handler(std::string const & tag_value,
			     std::string & value);

	std::string archive_path;
	std::string binary;
	std::vector<std::string> session;
	std::vector<std::string> session_exclude;
	std::vector<std::string> image;
	std::vector<std::string> image_exclude;
	std::vector<std::string> lib_image;
	comma_list<std::string> event;
	comma_list<int> count;
	comma_list<unsigned int> unitmask;
	comma_list<pid_t> tid;
	comma_list<pid_t> tgid;
	comma_list<int> cpu;
	
	std::vector<std::string> image_or_lib_image;

public: 
	
	extra_images extra_found_images;
};

#endif 
