/*
** 2011 March 18
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file contains a VFS "shim" - a layer that sits in between the
** pager and the real VFS.
**
** This particular shim enforces a multiplex system on DB files.  
** This shim shards/partitions a single DB file into smaller 
** "chunks" such that the total DB file size may exceed the maximum
** file size of the underlying file system.
**
*/

#ifndef _TEST_MULTIPLEX_H
#define _TEST_MULTIPLEX_H

#define MULTIPLEX_CTRL_ENABLE          214014
#define MULTIPLEX_CTRL_SET_CHUNK_SIZE  214015
#define MULTIPLEX_CTRL_SET_MAX_CHUNKS  214016

extern int sqlite3_multiplex_initialize(const char *zOrigVfsName, int makeDefault);

extern int sqlite3_multiplex_shutdown(void);

#endif
