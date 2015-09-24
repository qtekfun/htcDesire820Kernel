/**
 * @file image_errors.h
 * Report errors in images
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef IMAGE_ERRORS_H
#define IMAGE_ERRORS_H

#include <list>
#include <string>

class inverted_profile;
class extra_images;

enum image_error {
	image_ok = 0,
	image_not_found,
	image_unreadable,
	image_format_failure,
	image_multiple_match
};

void
report_image_error(std::string const & image, image_error error, bool fatal,
		   extra_images const & extra);

void report_image_error(inverted_profile const & profile, bool fatal,
			extra_images const & extra);

void report_image_errors(std::list<inverted_profile> const & plist,
			 extra_images const & extra);

#endif 
