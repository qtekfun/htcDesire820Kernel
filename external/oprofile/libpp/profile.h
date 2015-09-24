/**
 * @file profile.h
 * Encapsulation for samples files over all profile classes
 * belonging to the same binary image
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <map>
#include <iterator>

#include "odb.h"
#include "op_types.h"
#include "utility.h"
#include "populate_for_spu.h"

class opd_header;
class op_bfd;

class profile_t : noncopyable {
public:
	profile_t();

	
	bool empty() const { return !file_header.get(); }
 
	
	opd_header const & get_header() const {
		return *file_header;
	}

	static count_type sample_count(std::string const & filename);

	static enum profile_type is_spu_sample_file(std::string const & filename);

	void add_sample_file(std::string const & filename);

	
	void set_offset(op_bfd const & abfd);

	class const_iterator;
	typedef std::pair<const_iterator, const_iterator> iterator_pair;

	iterator_pair
	samples_range(odb_key_t start, odb_key_t end) const;

	
	iterator_pair samples_range() const;

private:
	
	
	static void
	open_sample_file(std::string const & filename, odb_t &);

	
	scoped_ptr<opd_header> file_header;

	
	typedef std::map<odb_key_t, count_type> ordered_samples_t;

	ordered_samples_t ordered_samples;

	u64 start_offset;
};


namespace std {
	template <>
		struct iterator_traits<profile_t::const_iterator> {
			typedef ptrdiff_t difference_type;
			typedef count_type value_type;
			typedef count_type * pointer;
			typedef count_type & reference;
			typedef input_iterator_tag iterator_category;
		};
}


class profile_t::const_iterator
{
	typedef ordered_samples_t::const_iterator iterator_t;
public:
	const_iterator() : start_offset(0) {}
	const_iterator(iterator_t it_, u64 start_offset_)
		: it(it_), start_offset(start_offset_) {}

	count_type operator*() const { return it->second; }
	const_iterator & operator++() { ++it; return *this; }

	odb_key_t vma() const { return it->first + start_offset; }
	count_type count() const { return **this; }

	bool operator!=(const_iterator const & rhs) const {
		return it != rhs.it;
	}
	bool operator==(const_iterator const & rhs) const {
		return it == rhs.it;
	}

private:
	iterator_t it;
	u64 start_offset;
};

#endif 
