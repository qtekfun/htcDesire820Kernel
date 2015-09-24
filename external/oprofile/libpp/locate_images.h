/**
 * @file locate_images.h
 * Location of binary images
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef LOCATE_IMAGES_H
#define LOCATE_IMAGES_H

#include <string>
#include <map>
#include <vector>

#include "image_errors.h"

class extra_images {
public:
	extra_images();

	
	
	void populate(std::vector<std::string> const & paths,
		      std::string const & archive_path,
		      std::string const & root_path);

	
	struct matcher {
		std::string const & value;
	public:
		explicit matcher(std::string const & v) : value(v) {}
		virtual ~matcher() {}
		
		virtual bool operator()(std::string const & str) const {
			return str == value;
		}
	};

	std::vector<std::string> const find(matcher const & match) const;

	
	std::vector<std::string> const find(std::string const & name) const;

	std::string const find_image_path(std::string const & image_name,
				image_error & error, bool fixup) const;

	
	std::string get_archive_path() const { return archive_path; }

	
	
	std::string strip_path_prefix(std::string const & image) const;

	
	int get_uid() const { return uid; }

private:
	void populate(std::vector<std::string> const & paths,
		      std::string const & prefix_path);

	std::string const locate_image(std::string const & image_name,
				image_error & error, bool fixup) const;

	typedef std::multimap<std::string, std::string> images_t;
	typedef images_t::value_type value_type;
	typedef images_t::const_iterator const_iterator;

	
	images_t images;
	
	std::string archive_path;
	
	
	std::string root_path;

	
	int uid;
	
	static int suid;
};

#endif 
