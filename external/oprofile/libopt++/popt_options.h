/**
 * @file popt_options.h
 * option parsing
 *
 * This provides a simple facility for adding command-line
 * options, and parsing them.
 *
 * You can add a number of options and then call parse_options()
 * to process them, for example :
 *
 * \code
 * bool allow_frob;
 * string frob;
 * static popt::option allow_frob_opt(allow_frob, "allow-frob", 'a', "allow frobs");
 * static popt::option frob_opt(frob, "frob", 'f', "what to frob", "name");
 *
 * ...
 * popt::parse_options(argc, argv, add_params);
 * \endcode
 *
 * Note than if you try to implement an option for an unsupported type  like :
 * \code
 * static unsigned int i;
 * static popt::option i_opt(i, ....);
 * \endcode
 * you don't get a compile time error but a link time error.
 *
 * The call to parse_options() will fill in allow_frob and frob, if they
 * are passed to the program (myfrobber --allow-frob --frob foo), and place
 * any left over command line arguments in the add_params vector. Note
 * that the template parameter denotes the type of the option argument.
 *
 * When the template parameter type is bool, option starting with "no-" prefix
 * are implicitely considered as negated before writing the associated bool so
 * this will work as expected:
 * \code
 * bool demangle;
 * popt::option(demangle, "demangle", 'd', "demangle C++ symbols"),
 * popt::option(demangle, "no-demangle", '\0', "don't demangle C++ symbols"),
 * \endcode
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef POPT_OPTIONS_H
#define POPT_OPTIONS_H

#include <string>
#include <vector>

namespace popt {
 
void parse_options(int argc, char const ** argv,
                   std::vector<std::string> & additional_params);

class option_base;

class option {
public:
	template <class T> option(T &, char const * option_name,
	                          char short_name, char const * help_str,
	                          char const * arg_help_str);

	option(bool &, char const * option_name,
	       char short_name, char const * help_str);

	~option();

private:
	option_base * the_option;
};


template <> option::option(int &, char const * option_name, char short_name,
                           char const * help_str, char const * arg_help_str);
template <> option::option(std::string &, char const * option_name,
                           char short_name, char const * help_str,
                           char const * arg_help_str);
template <> option::option(std::vector<std::string> &,
                           char const * option_name, char short_name,
                           char const * help_str, char const * arg_help_str);

} 

#endif 
