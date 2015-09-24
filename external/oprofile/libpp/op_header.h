/**
 * @file op_header.h
 * various free function acting on a sample file header
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_HEADER_H
#define OP_HEADER_H

#include <iosfwd>
#include <string>

#include "op_sample_file.h"

void op_check_header(opd_header const & h1, opd_header const & h2,
                     std::string const & filename);

bool is_jit_sample(std::string const & filename);

void check_mtime(std::string const & file, opd_header const & header);

opd_header const read_header(std::string const & sample_filename);

std::string const describe_header(opd_header const & header);

std::string const describe_cpu(opd_header const & header);

#endif 
