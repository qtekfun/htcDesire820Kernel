/**
 * @file daemon/opd_pipe.h
 * Functions handling the $SESSIONDIR/opd_pipe FIFO special file.
 * NOTE: This code is dealing with potencially insecure input.
 *
 * @remark Copyright 2008 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Daniel Hansel
 */

#ifndef OPD_PIPE_H_
#define OPD_PIPE_H_
 
void opd_create_pipe(void);

void opd_open_pipe(void);

void opd_close_pipe(void);

int is_jitconv_requested(void);

#endif 
