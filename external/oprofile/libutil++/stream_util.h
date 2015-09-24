/**
 * @file stream_util.h
 * C++ stream utility
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef STREAM_UTIL_H
#define STREAM_UTIL_H

#include <iostream>

class io_state {
public:
	io_state(std::ios & stream);
	
	~io_state();
private:
	std::ios & stream;

	std::ios::fmtflags format;
	std::streamsize precision;
	char fill;
};

#endif 
