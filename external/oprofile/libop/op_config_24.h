/**
 * @file op_config_24.h
 *
 * Parameters a user may want to change
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_CONFIG_24_H
#define OP_CONFIG_24_H

#define OP_MOUNT "/proc/sys/dev/oprofile/"

extern char op_device[];
extern char op_note_device[];
extern char op_hash_device[];


#define OP_DEFAULT_BUF_SIZE 65536
#define OP_PRE_WATERMARK(buffer_size)			\
	(((buffer_size) / 8) < OP_MIN_PRE_WATERMARK	\
		? OP_MIN_PRE_WATERMARK			\
		: (buffer_size) / 8)
#define OP_MIN_PRE_WATERMARK 8192
#define OP_MAX_BUF_SIZE	1048576
#define OP_MIN_BUF_SIZE	(32768 + OP_PRE_WATERMARK(32768))

#define OP_DEFAULT_NOTE_SIZE 16384
#define OP_PRE_NOTE_WATERMARK(note_size)		\
	(((note_size) / 32) < OP_MIN_NOTE_PRE_WATERMARK	\
		? OP_MIN_NOTE_PRE_WATERMARK		\
		: (note_size) / 32)
#define OP_MIN_NOTE_PRE_WATERMARK 512
#define OP_MAX_NOTE_TABLE_SIZE	1048576
#define OP_MIN_NOTE_TABLE_SIZE	(1024 + OP_PRE_NOTE_WATERMARK(1024))

#define OP_MAX_RTC_COUNT	4096
#define OP_MIN_RTC_COUNT	2


#define OP_HASH_MAP_NR 4093

#define POOL_SIZE 65536

#ifndef NR_CPUS
#define NR_CPUS 32
#endif

#endif 
