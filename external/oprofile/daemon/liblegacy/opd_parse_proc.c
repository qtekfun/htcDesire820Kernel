/**
 * @file opd_parse_proc.c
 * Parsing of /proc/#pid
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#include "op_libiberty.h"

#include "opd_parse_proc.h"
#include "opd_proc.h"
#include "opd_mapping.h"
#include "opd_image.h"
#include "opd_printf.h"

#include "op_file.h"
#include "op_fileio.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int opd_add_ascii_map(struct opd_proc * proc, char const * line,
			     char * const image_name)
{
	unsigned long offset, start, end;
	struct opd_image * image;
	char const * cp = line;

	
	while (*cp && *cp != ' ')
		cp++;

	
	if (!*cp || (!*(++cp)) || (!*(++cp)) || (*(++cp) != 'x'))
		return 0;

	
	if (sscanf(line, "%lx-%lx", &start, &end) != 2)
		return 0;

	
	cp += 2;

	
	if (sscanf(cp, "%lx", &offset) != 1)
		return 0;

	while (*cp && *cp != '/')
		cp++;

	if (!*cp)
		return 0;

	image = opd_get_image(cp, image_name, 0, proc->tid, proc->tgid);
	if (!image)
		return 0;

	opd_add_mapping(proc, image, start, offset, end);

	return 1;
}


static void opd_get_ascii_maps(struct opd_proc * proc)
{
	FILE * fp;
	char mapsfile[20] = "/proc/";
	char * line;
	char exe_name[20];
	char * image_name;
	struct list_head * pos;

	snprintf(mapsfile + 6, 6, "%hu", proc->tid);

	strcpy(exe_name, mapsfile);

	strcat(mapsfile, "/maps");

	fp = op_try_open_file(mapsfile, "r");
	if (!fp)
		return;

	strcat(exe_name, "/exe");
	image_name = xmalloc(PATH_MAX);
	if (!realpath(exe_name, image_name))
		
		strcpy(image_name, exe_name);

	verbprintf(vmisc, "image name %s for pid %u %u\n", image_name, proc->tid, proc->tgid);

	while (1) {
		line = op_get_line(fp);
		if (!line)
			break;

		opd_add_ascii_map(proc, line, image_name);
		free(line);
	}

	list_for_each(pos, &proc->maps) {
		struct opd_map * map = list_entry(pos, struct opd_map, next);
		if (!strcmp(map->image->name, image_name)) {
			if (pos != proc->maps.next) {
				fprintf(stderr, "swap map for image %s from %s to %s\n", image_name, proc->name, map->image->name);
				free((char *)proc->name);
				proc->name = xstrdup(map->image->name);
			}
			break;
		}
	}

	if (list_empty(&proc->maps)) {
		struct opd_image * image = opd_get_image(image_name,
                                       image_name, 0, proc->tid, proc->tgid);
		if (image)
			opd_add_mapping(proc, image, 0, 0, 0);
	}

	if (image_name)
		free(image_name);

	op_close_file(fp);
}


static u32 read_tgid(u32 tid)
{
	char status_file[30] = "/proc/";
	char * line;
	FILE * fp;
	u32 tgid;

	snprintf(status_file + 6, 6, "%hu", tid);

	strcat(status_file, "/status");

	fp = op_try_open_file(status_file, "r");
	if (!fp)
		return 0;

	while (1) {
		line = op_get_line(fp);
		if (!line)
			break;

		if (sscanf(line, "Tgid: %u", &tgid) == 1) {
			free(line);
			op_close_file(fp);
			return tgid;
		}
		free(line);
	}

	op_close_file(fp);

	return 0;
}


void opd_get_ascii_procs(void)
{
	DIR * dir;
	struct dirent * dirent;
	struct opd_proc * proc;
	u32 pid;

	if (!(dir = opendir("/proc"))) {
		perror("oprofiled: /proc directory could not be opened. ");
		exit(EXIT_FAILURE);
	}

	while ((dirent = readdir(dir))) {
		if (sscanf(dirent->d_name, "%u", &pid) == 1) {
			u32 tgid = read_tgid(pid);
			verbprintf(vmisc, "ASCII added %u %u\n", pid, tgid);
			proc = opd_get_proc(pid, tgid);
			if (!proc)
				proc = opd_new_proc(pid, tgid);
			opd_get_ascii_maps(proc);
		}
	}

	closedir(dir);
}
