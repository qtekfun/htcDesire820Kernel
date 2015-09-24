/**
 * @file sparse_array.h
 * Auto-expanding sparse array type
 *
 * @remark Copyright 2007 OProfile authors
 * @remark Copyright (c) International Business Machines, 2007.
 * @remark Read the file COPYING
 *
 * @author Dave Nomura <dcnltc@us.ibm.com>
 */

#ifndef SPARSE_ARRAY_H
#define SPARSE_ARRAY_H

template <typename I, typename T> class sparse_array {
public:
	typedef std::map<I, T> container_type;
	typedef typename container_type::size_type size_type;

	T operator[](size_type index) const {
		typename container_type::const_iterator it = container.find(index);
		if (it != container.end())
			return it->second;
		else
			return 0;
	}


	T & operator[](size_type index) {
		return container[index];
	}


	sparse_array & operator+=(sparse_array const & rhs) {
		typename container_type::const_iterator it = rhs.container.begin();
		typename container_type::const_iterator it_end = rhs.container.end();
		for ( ; it != it_end; it++)
			container[it->first] += it->second;

		return *this;
	}


	sparse_array & operator-=(sparse_array const & rhs) {
		typename container_type::const_iterator it = rhs.container.begin();
		typename container_type::const_iterator it_end = rhs.container.end();
		for ( ; it != it_end; it++)
			container[it->first] -= it->second;

		return *this;
	}


	size_type size() const {
		if (container.size() == 0)
			return 0;
		typename container_type::const_iterator last = container.end();
		--last;
		return last->first + 1;
	}


	
	bool zero() const {
		typename container_type::const_iterator it = container.begin();
		typename container_type::const_iterator it_end = container.end();
		for ( ; it != it_end; it++)
			if (it->second != 0)
				return false;
		return true;
	}

private:
	container_type container;
};

#endif 
