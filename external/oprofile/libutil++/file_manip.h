/**
 * @file file_manip.h
 * Useful file management helpers
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef FILE_MANIP_H
#define FILE_MANIP_H

#include <string>
#include <list>


bool copy_file(std::string const & source, std::string const & destination);

bool is_directory(std::string const & dirname);

bool is_files_identical(std::string const & file1, std::string const & file2);

std::string const op_realpath(std::string const & name);

bool op_file_readable(std::string const & file);

bool create_file_list(std::list<std::string> & file_list,
		      std::string const & base_dir,
		      std::string const & filter = "*",
		      bool recursive = false);

std::string op_dirname(std::string const & file_name);

std::string op_basename(std::string const & path_name);

#endif 
