/**
 * @file format_output.h
 * outputting format for symbol lists
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef FORMAT_OUTPUT_H
#define FORMAT_OUTPUT_H

#include "config.h"

#include <string>
#include <map>
#include <iosfwd>

#include "format_flags.h"
#include "symbol.h"
#include "string_filter.h"
#include "xml_output.h"

class symbol_entry;
class sample_entry;
class callgraph_container;
class profile_container;
class diff_container;
class extra_images;
class op_bfd;

struct profile_classes;
extern profile_classes classes;

namespace format_output {

class formatter {
public:
	formatter(extra_images const & extra);
	virtual ~formatter();

	
	void add_format(format_flags flag);

	
	void show_header(bool);
	
	void vma_format_64bit(bool);
	
	void show_long_filenames(bool);
	
	void show_global_percent(bool);

	void set_nr_classes(size_t nr_classes);

	
	
	void output_header(std::ostream & out);

protected:
	struct counts_t {
		
		count_array_t total;
		
		count_array_t cumulated_samples;
		
		count_array_t cumulated_percent;
		
		count_array_t cumulated_percent_details;
	};

	
	struct field_datum {
		field_datum(symbol_entry const & sym,
		            sample_entry const & s,
			    size_t pc, counts_t & c,
			    extra_images const & extra, double d = 0.0)
			: symbol(sym), sample(s), pclass(pc),
			  counts(c), extra(extra), diff(d) {}
		symbol_entry const & symbol;
		sample_entry const & sample;
		size_t pclass;
		counts_t & counts;
		extra_images const & extra;
		double diff;
	};
 
	
	typedef std::string (formatter::*fct_format)(field_datum const &);
 
	
	std::string format_vma(field_datum const &);
	std::string format_symb_name(field_datum const &);
	std::string format_image_name(field_datum const &);
	std::string format_app_name(field_datum const &);
	std::string format_linenr_info(field_datum const &);
	std::string format_nr_samples(field_datum const &);
	std::string format_nr_cumulated_samples(field_datum const &);
	std::string format_percent(field_datum const &);
	std::string format_cumulated_percent(field_datum const &);
	std::string format_percent_details(field_datum const &);
	std::string format_cumulated_percent_details(field_datum const &);
	std::string format_diff(field_datum const &);
	
 
	
	struct field_description {
		field_description() {}
		field_description(std::size_t w, std::string h,
				  fct_format f)
			: width(w), header_name(h), formatter(f) {}
 
		std::size_t width;
		std::string header_name;
		fct_format formatter;
	};
 
	typedef std::map<format_flags, field_description> format_map_t;

	
	void do_output(std::ostream & out, symbol_entry const & symbol,
		      sample_entry const & sample, counts_t & c,
	              diff_array_t const & = diff_array_t(),
	              bool hide_immutable_field = false);
 
	
	size_t output_header_field(std::ostream & out, format_flags fl,
	                           size_t padding);

	
	size_t output_field(std::ostream & out, field_datum const & datum,
			   format_flags fl, size_t padding,
			   bool hide_immutable);
 
	
	format_map_t format_map;

	
	size_t nr_classes;

	
	counts_t counts;

	
	format_flags flags;
	
	bool vma_64;
	
	bool long_filenames;
	
	bool need_header;
	
	
	bool global_percent;

	
	
	extra_images const & extra_found_images;
};
 

class opreport_formatter : public formatter {
public:
	
	opreport_formatter(profile_container const & profile);

	void output(std::ostream & out, symbol_collection const & syms);

	
	void show_details(bool);

private:
 
	void output(std::ostream & out, symbol_entry const * symb);

	
	void output_details(std::ostream & out, symbol_entry const * symb);
 
	
	profile_container const & profile;
 
	
	bool need_details;
};


class cg_formatter : public formatter {
public:
	
	cg_formatter(callgraph_container const & profile);

	void output(std::ostream & out, symbol_collection const & syms);
};

class diff_formatter : public formatter {
public:
	
	diff_formatter(diff_container const & profile,
		       extra_images const & extra);

	void output(std::ostream & out, diff_collection const & syms);

private:
	
	void output(std::ostream & out, diff_symbol const & sym);

};


class xml_formatter : public formatter {
public:
	
	xml_formatter(profile_container const * profile,
		      symbol_collection & symbols, extra_images const & extra,
		      string_filter const & symbol_filter);

	
	void output(std::ostream & out);

	virtual void output_symbol(std::ostream & out,
		symbol_entry const * symb, size_t lo, size_t hi,
		bool is_module);

	
	std::string output_symbol_details(symbol_entry const * symb,
		size_t & detail_index, size_t const lo, size_t const hi);

	
	void show_details(bool);

	
	void output_symbol_data(std::ostream & out);

private:
	
	profile_container const * profile;
 
	
	symbol_collection & symbols;

	
	bool need_details;

	
	size_t detail_count;

	
	
	string_filter const & symbol_filter;

	void output_sample_data(std::ostream & out,
		sample_entry const & sample, size_t count);

	
	void output_attribute(std::ostream & out, field_datum const & datum,
			      format_flags fl, tag_t tag);

	
	
	bool get_bfd_object(symbol_entry const * symb, op_bfd * & abfd) const;

	void output_the_symbol_data(std::ostream & out,
		symbol_entry const * symb, op_bfd * & abfd);

	void output_cg_children(std::ostream & out,
		cg_symbol::children const cg_symb, op_bfd * & abfd);
};

class xml_cg_formatter : public xml_formatter {
public:
	
	xml_cg_formatter(callgraph_container const & callgraph,
		symbol_collection & symbols, string_filter const & sf);

	virtual void output_symbol(std::ostream & out,
		symbol_entry const * symb, size_t lo, size_t hi, bool is_module);

private:
	
	callgraph_container const & callgraph;

	void output_symbol_core(std::ostream & out,
		cg_symbol::children const cg_symb,
		std::string const selfname, std::string const qname,
		size_t lo, size_t hi, bool is_module, tag_t tag);
};

} 


#endif 
