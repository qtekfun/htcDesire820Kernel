/**
 * @file name_storage.h
 * Type-safe unique storage of global names (filenames and symbols)
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef NAME_STORAGE_H
#define NAME_STORAGE_H

#include <string>

#include "unique_storage.h"

class extra_images;

struct stored_name {
	stored_name(std::string const & n = std::string())
		: name(n) {}

	bool operator<(stored_name const & rhs) const {
		return name < rhs.name;
	}

	std::string name;
	mutable std::string name_processed;
};


template <typename I> struct name_storage : unique_storage<I, stored_name> {

	typedef typename unique_storage<I, stored_name>::id_value id_value;

	std::string const & name(id_value const & id) const {
		return unique_storage<I, stored_name>::get(id).name;
	}
};


class debug_name_tag;
typedef name_storage<debug_name_tag>::id_value debug_name_id;

struct debug_name_storage : name_storage<debug_name_tag> {
	
	std::string const & basename(debug_name_id id) const;
};

struct stored_filename {
	stored_filename(std::string const & n = std::string())
		: filename(n), extra_images_uid(0) {}

	bool operator<(stored_filename const & rhs) const {
		return filename < rhs.filename;
	}

	std::string filename;
	mutable std::string base_filename;
	mutable std::string real_filename;
	mutable std::string real_base_filename;
	mutable int extra_images_uid;
};

template <typename I> 
struct filename_storage : unique_storage<I, stored_filename> {

	typedef typename unique_storage<I, stored_filename>::id_value id_value;

	std::string const & name(id_value const & id) const {
		return unique_storage<I, stored_filename>::get(id).filename;
	}
};

class image_name_tag;
typedef filename_storage<image_name_tag>::id_value image_name_id;

struct image_name_storage : filename_storage<image_name_tag> {
	enum image_name_type {
		
		int_basename,
		
		int_filename,
		
		int_real_basename,
		
		
		int_real_filename,
	};

	std::string const & get_name(image_name_id id,
				     image_name_type type,
				     extra_images const & extra) const;

	
	std::string const & basename(image_name_id) const;
};


class symbol_name_tag;
typedef name_storage<symbol_name_tag>::id_value symbol_name_id;

struct symbol_name_storage : name_storage<symbol_name_tag> {
	
	std::string const & demangle(symbol_name_id id) const;
};


extern image_name_storage image_names;

extern debug_name_storage debug_names;

extern symbol_name_storage symbol_names;


template<> inline bool
debug_name_id::operator<(debug_name_id const & rhs) const
{
	return debug_names.name(*this) < debug_names.name(rhs);
}

#endif 
