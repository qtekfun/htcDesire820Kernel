/*
 * Error reporting
 *
 * Copyright (C) 2010 Red Hat Inc.
 *
 * Authors:
 *  Markus Armbruster <armbru@redhat.com>,
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include <stdio.h>
#include "monitor.h"

void error_vprintf(const char *fmt, va_list ap)
{
    if (cur_mon) {
        monitor_vprintf(cur_mon, fmt, ap);
    } else {
        vfprintf(stderr, fmt, ap);
    }
}

void error_printf(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    error_vprintf(fmt, ap);
    va_end(ap);
}

void error_printf_unless_qmp(const char *fmt, ...)
{
    va_list ap;

    if (!monitor_cur_is_qmp()) {
        va_start(ap, fmt);
        error_vprintf(fmt, ap);
        va_end(ap);
    }
}

static Location std_loc = {
    .kind = LOC_NONE
};
static Location *cur_loc = &std_loc;

Location *loc_push_restore(Location *loc)
{
    assert(!loc->prev);
    loc->prev = cur_loc;
    cur_loc = loc;
    return loc;
}

Location *loc_push_none(Location *loc)
{
    loc->kind = LOC_NONE;
    loc->prev = NULL;
    return loc_push_restore(loc);
}

Location *loc_pop(Location *loc)
{
    assert(cur_loc == loc && loc->prev);
    cur_loc = loc->prev;
    loc->prev = NULL;
    return loc;
}

Location *loc_save(Location *loc)
{
    *loc = *cur_loc;
    loc->prev = NULL;
    return loc;
}

void loc_restore(Location *loc)
{
    Location *prev = cur_loc->prev;
    assert(!loc->prev);
    *cur_loc = *loc;
    cur_loc->prev = prev;
}

void loc_set_none(void)
{
    cur_loc->kind = LOC_NONE;
}

void loc_set_cmdline(char **argv, int idx, int cnt)
{
    cur_loc->kind = LOC_CMDLINE;
    cur_loc->num = cnt;
    cur_loc->ptr = argv + idx;
}

void loc_set_file(const char *fname, int lno)
{
    assert (fname || cur_loc->kind == LOC_FILE);
    cur_loc->kind = LOC_FILE;
    cur_loc->num = lno;
    if (fname) {
        cur_loc->ptr = fname;
    }
}

static const char *progname;

void error_set_progname(const char *argv0)
{
    const char *p = strrchr(argv0, '/');
    progname = p ? p + 1 : argv0;
}

void error_print_loc(void)
{
    const char *sep = "";
    int i;
    const char *const *argp;

    if (!cur_mon && progname) {
        fprintf(stderr, "%s:", progname);
        sep = " ";
    }
    switch (cur_loc->kind) {
    case LOC_CMDLINE:
        argp = cur_loc->ptr;
        for (i = 0; i < cur_loc->num; i++) {
            error_printf("%s%s", sep, argp[i]);
            sep = " ";
        }
        error_printf(": ");
        break;
    case LOC_FILE:
        error_printf("%s:", (const char *)cur_loc->ptr);
        if (cur_loc->num) {
            error_printf("%d:", cur_loc->num);
        }
        error_printf(" ");
        break;
    default:
        error_printf("%s", sep);
    }
}

void error_report(const char *fmt, ...)
{
    va_list ap;

    error_print_loc();
    va_start(ap, fmt);
    error_vprintf(fmt, ap);
    va_end(ap);
    error_printf("\n");
}
