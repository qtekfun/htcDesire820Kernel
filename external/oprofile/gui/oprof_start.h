/**
 * @file oprof_start.h
 * The GUI start main class
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 * @author John Levon
 */

#ifndef OPROF_START_H
#define OPROF_START_H

#include <vector>
#include <map>
#include <set>

#include "ui/oprof_start.base.h"
#include "oprof_start_config.h"

#include "op_events.h"

class QIntValidator;
class QListViewItem;
class QTimerEvent;

struct op_event_descr {
	op_event_descr();

	
	uint counter_mask;
	
	u32 val;
	
	op_unit_mask const * unit;
	
	std::string name;
	
	std::string help_str;
	
	uint min_count;
};

class oprof_start : public oprof_start_base
{
	Q_OBJECT

public:
	oprof_start();

protected slots:
	
	void choose_kernel_filename();
	
	void on_flush_profiler_data();
	
	void on_start_profiler();
	
	void on_stop_profiler();
	
	void event_selected();
	
	void event_over(QListViewItem *);
	
	void on_separate_kernel_cb_changed(int);
	
	void on_reset_sample_files();

	
	void accept();

	
	void closeEvent(QCloseEvent * e);

	
	void timerEvent(QTimerEvent * e);

private:
	
	void fill_events_listbox();

	
	void fill_events();

	
	op_event_descr const & locate_event(std::string const & name) const;

	
	void record_selected_event_config();
	
	bool record_config();

	
	void get_unit_mask_part(op_event_descr const & descr, uint num, bool selected, uint & mask);
	
	uint get_unit_mask(op_event_descr const & descr);
	
	void setup_unit_masks(op_event_descr const & descr);

	
	uint max_perf_count() const;

	
	void display_event(op_event_descr const & descrp);

	
	void hide_masks(void);

	
	void read_set_events();
	
	void setup_default_event();
	
	void load_config_file();
	
	bool save_config();

	
	void draw_event_list();

	
	bool is_selectable_event(QListViewItem * item);

	
	bool alloc_selected_events() const;

	
	QIntValidator* event_count_validator;

	
	std::vector<op_event_descr> v_events;

	
	typedef std::map<std::string, event_setting> event_setting_map;
	event_setting_map event_cfgs;

	
	
	
	std::set<QListViewItem *> selected_events;
	QListViewItem * current_event;

	
	config_setting config;

	
	std::string user_dir;

	
	op_cpu cpu_type;

	
	double cpu_speed;

	
	uint op_nr_counters;

	
	unsigned long total_nr_interrupts;
};

#endif 
