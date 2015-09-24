/**
 * @file cverb.h
 * verbose output stream
 *
 * @remark Copyright 2002, 2004 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef CVERB_H
#define CVERB_H

#include <iosfwd>
#include <string>
#include <vector>

struct cverb_object { };

extern cverb_object cverb;

class verbose {
	
	friend std::ostream & operator<<(cverb_object &, verbose const &);
public:
	verbose(char const * name);

	verbose operator|(verbose const &);
	verbose operator&(verbose const &);

	
	static bool setup(std::string const &);
	
	static bool setup(std::vector<std::string> const & args);
private:
	bool set;
};

extern verbose vlevel1; 
extern verbose vdebug;  
extern verbose vstats;  
extern verbose vsfile;  
extern verbose vxml;  

#endif 
