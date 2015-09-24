/**
 * @file op_exception.h
 * exception base class
 *
 * This provide simple base class for exception object. All
 * exception are derived from directly or indirectly from
 * std::exception. This class are not itended to be catched
 * in your code except at top level, derive what you want
 * and catch derived class rather.
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef OP_EXCEPTION_H
#define OP_EXCEPTION_H

#include <stdexcept>
#include <string>


class op_exception : public std::exception {
public:
	explicit op_exception(std::string const& msg);
	~op_exception() throw() = 0;

	char const * what() const throw();
private:
	std::string message;
};


struct op_fatal_error : op_exception
{
	explicit op_fatal_error(std::string const & msg);
};

struct op_runtime_error : std::runtime_error
{
	explicit op_runtime_error(std::string const & err);
	op_runtime_error(std::string const & err, int cerrno);
	~op_runtime_error() throw();
};


#endif 
