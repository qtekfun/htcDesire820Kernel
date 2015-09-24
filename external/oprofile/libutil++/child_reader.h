/**
 * @file child_reader.h
 * Facility for reading from child processes
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef CHILD_READER_H
#define CHILD_READER_H

#include <sys/types.h>

#include <vector>
#include <string>

class child_reader {
public:
	child_reader(std::string const & cmd,
		std::vector<std::string> const & args);

	~child_reader();

	bool getline(std::string & result);

	bool get_data(std::ostream & out, std::ostream & err);

	int terminate_process();

	int error() const { return first_error; }

	std::string error_str() const;

private:
	
	void exec_command(std::string const & cmd,
			  std::vector<std::string> const & args);
	
	
	bool block_read();

	int fd1;
	int fd2;
	ssize_t pos1;
	ssize_t end1;
	ssize_t pos2;
	ssize_t end2;
	pid_t pid;
	int first_error;
	
	
	char * buf2;
	ssize_t sz_buf2;
	char * buf1;
	std::string process_name;
	bool is_terminated;
	bool terminate_on_exception;
	bool forked;
};

#endif 
