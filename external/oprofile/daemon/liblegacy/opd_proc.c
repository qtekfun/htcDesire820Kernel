/**
 * @file opd_proc.c
 * Management of processes
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#include "op_hw_config.h"
#include "opd_proc.h"
#include "opd_image.h"
#include "opd_mapping.h"
#include "opd_sample_files.h"
#include "opd_kernel.h"
#include "opd_24_stats.h"
#include "opd_printf.h"
#include "oprofiled.h"

#include "op_interface.h"
#include "op_libiberty.h"

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPD_MAX_PROC_HASH 1024

extern int cpu_number;

static struct list_head opd_procs[OPD_MAX_PROC_HASH];

static int nr_procs;


void opd_init_procs(void)
{
	int i;
	for (i = 0; i < OPD_MAX_PROC_HASH; i++)
		list_init(&opd_procs[i]);
}


int opd_get_nr_procs(void)
{
	return nr_procs;
}


inline static uint proc_hash(pid_t tid)
{
	
	return ((tid >> 4) ^ (tid)) % OPD_MAX_PROC_HASH;
}


struct opd_proc * opd_new_proc(pid_t tid, pid_t tgid)
{
	struct opd_proc * proc;

	nr_procs++;
	proc = xmalloc(sizeof(struct opd_proc));
	list_init(&proc->maps);
	proc->name = NULL;
	proc->tid = tid;
	proc->tgid = tgid;
	proc->dead = 0;
	proc->accessed = 0;
	list_add(&proc->next, &opd_procs[proc_hash(tid)]);
	return proc;
}


struct opd_proc * opd_get_proc(pid_t tid, pid_t tgid)
{
	struct opd_proc * proc;
	uint hash = proc_hash(tid);
	struct list_head * pos, *pos2;

	opd_24_stats[OPD_PROC_QUEUE_ACCESS]++;
	list_for_each_safe(pos, pos2, &opd_procs[hash]) {
		opd_24_stats[OPD_PROC_QUEUE_DEPTH]++;
		proc = list_entry(pos, struct opd_proc, next);
		if (tid == proc->tid && tgid == proc->tgid) {
			
			list_del(&proc->next);
			list_add(&proc->next, &opd_procs[hash]);
			return proc;
		}
	}

	return NULL;
}


inline static void
verb_show_sample(unsigned long offset, struct opd_map * map)
{
	verbprintf(vsamples, "DO_PUT_SAMPLE : calc offset 0x%.8lx, "
		"map start 0x%.8lx, end 0x%.8lx, offset 0x%.8lx, name \"%s\"\n",
		offset, map->start, map->end, map->offset, 
		map->image->name);
}


void opd_put_image_sample(struct opd_image * image, unsigned long offset,
                          u32 counter)
{
	struct opd_24_sfile * sfile;
	int err;

	if (image->ignored)
		return;

	if (!image->sfiles[cpu_number]) {
		image->sfiles[cpu_number] =
			xcalloc(OP_MAX_COUNTERS, sizeof(struct op_24_sfile *));
	}
	sfile = image->sfiles[cpu_number][counter];

	if (!sfile || !odb_open_count(&sfile->sample_file)) {
		if (opd_open_24_sample_file(image, counter, cpu_number)) {
			
			opd_24_stats[OPD_LOST_SAMPLEFILE]++;
			return;
		}
		sfile = image->sfiles[cpu_number][counter];
	}

	err = odb_update_node(&sfile->sample_file, offset);
	if (err) {
		fprintf(stderr, "%s\n", strerror(err));
		abort();
	}

	opd_24_sfile_lru(sfile);
}


static int opd_lookup_maps(struct opd_proc * proc,
			struct op_sample const * sample)
{
	struct list_head * pos;

	proc->accessed = 1;

	opd_24_stats[OPD_MAP_ARRAY_ACCESS]++;
	list_for_each(pos, &proc->maps) {
		struct opd_map * map = list_entry(pos, struct opd_map, next);
		if (opd_is_in_map(map, sample->eip)) {
			unsigned long offset = opd_map_offset(map, sample->eip);
			if (map->image != NULL) {
				verb_show_sample(offset, map);
				opd_put_image_sample(map->image, offset, sample->counter);
			}
			opd_24_stats[OPD_PROCESS]++;
			return 1;
		}
		opd_24_stats[OPD_MAP_ARRAY_DEPTH]++;
	}

	return 0;
}


void opd_put_sample(struct op_sample const * sample)
{
	struct opd_proc * proc;
	int in_kernel_eip = opd_eip_is_kernel(sample->eip);

	opd_24_stats[OPD_SAMPLES]++;

	verbprintf(vsamples, "DO_PUT_SAMPLE: c%d, EIP 0x%.8lx, tgid %.6d pid %.6d\n",
		sample->counter, sample->eip, sample->tgid, sample->pid);

	if (!separate_kernel && in_kernel_eip) {
		opd_handle_kernel_sample(sample->eip, sample->counter);
		return;
	}

	if (!(proc = opd_get_proc(sample->pid, sample->tgid))) {
		if (in_kernel_eip || no_vmlinux) {
			opd_handle_kernel_sample(sample->eip, sample->counter);
		} else {
			verbprintf(vmisc, "No proc info for tgid %.6d pid %.6d.\n",
                                   sample->tgid, sample->pid);
			opd_24_stats[OPD_LOST_PROCESS]++;
		}
		return;
	}

	if (opd_lookup_maps(proc, sample))
		return;

	if (in_kernel_eip) {
		opd_add_kernel_map(proc, sample->eip);
		if (opd_lookup_maps(proc, sample))
			return;
	}

	
	verbprintf(vsamples, "Couldn't find map for pid %.6d, EIP 0x%.8lx.\n",
		   sample->pid, sample->eip);
	opd_24_stats[OPD_LOST_MAP_PROCESS]++;
}


void opd_handle_fork(struct op_note const * note)
{
	struct opd_proc * old;
	struct opd_proc * proc;
	struct list_head * pos;

	verbprintf(vmisc, "DO_FORK: from %d, %d to %ld, %ld\n", note->pid, note->tgid,
	           note->addr, note->len);

	old = opd_get_proc(note->pid, note->tgid);

	if (opd_get_proc(note->addr, note->len))
		return;

	
	proc = opd_new_proc(note->addr, note->len);

	if (!old)
		return;

	
	list_for_each(pos, &old->maps) {
		struct opd_map * map = list_entry(pos, struct opd_map, next);
		if (!separate_thread) {
			opd_add_mapping(proc, map->image, map->start,
			                map->offset, map->end);
		} else {
			struct opd_image * image = 
				opd_get_image(map->image->name, old->name,
				map->image->kernel, note->addr, note->len);
			opd_add_mapping(proc, image, map->start, map->offset,
			                map->end);
		}
	}
}


void opd_handle_exec(pid_t tid, pid_t tgid)
{
	struct opd_proc * proc;

	verbprintf(vmisc, "DO_EXEC: pid %u %u\n", tid, tgid);

	proc = opd_get_proc(tid, tgid);
	if (proc) {
		opd_kill_maps(proc);
		
		free((char *)proc->name);
		proc->name = NULL;
	} else {
		opd_new_proc(tid, tgid);
	}
}


void opd_handle_exit(struct op_note const * note)
{
	struct opd_proc * proc;

	verbprintf(vmisc, "DO_EXIT: process %d\n", note->pid);

	proc = opd_get_proc(note->pid, note->tgid);
	if (proc) {
		proc->dead = 1;
		proc->accessed = 1;
	} else {
		verbprintf(vmisc, "unknown proc %u just exited.\n", note->pid);
	}
}


typedef void (*opd_proc_cb)(struct opd_proc *);

static void opd_for_each_proc(opd_proc_cb proc_cb)
{
	struct list_head * pos;
	struct list_head * pos2;
	int i;

	for (i = 0; i < OPD_MAX_PROC_HASH; ++i) {
		list_for_each_safe(pos, pos2, &opd_procs[i]) {
			struct opd_proc * proc =
				list_entry(pos, struct opd_proc, next);
			proc_cb(proc);
		}
	}
}


static void opd_delete_proc(struct opd_proc * proc)
{
	--nr_procs;
	list_del(&proc->next);
	opd_kill_maps(proc);
	if (proc->name)
		free((char *)proc->name);
	free(proc);
}


void opd_proc_cleanup(void)
{
	opd_for_each_proc(opd_delete_proc);
}


static void opd_age_proc(struct opd_proc * proc)
{
	
	if (proc->dead) {
		proc->dead += proc->accessed;
		proc->accessed = 0;
		if (--proc->dead == 0)
			opd_delete_proc(proc);
	}
}


void opd_age_procs(void)
{
	opd_for_each_proc(opd_age_proc);
}


static void opd_remove_kernel_mapping(struct opd_proc * proc)
{
	struct list_head * pos, * pos2;

	list_for_each_safe(pos, pos2, &proc->maps) {
		struct opd_map * map = list_entry(pos, struct opd_map, next);
		if (opd_eip_is_kernel(map->start + map->offset)) {
			list_del(pos);
			opd_delete_image(map->image);
			free(map);
		}
	}
}


void opd_clear_kernel_mapping(void)
{
	opd_for_each_proc(opd_remove_kernel_mapping);
}
