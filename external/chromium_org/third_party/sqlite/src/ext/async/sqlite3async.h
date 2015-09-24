
#ifndef __SQLITEASYNC_H_
#define __SQLITEASYNC_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#define SQLITEASYNC_VFSNAME "sqlite3async"


 
int sqlite3async_initialize(const char *zParent, int isDefault);

 
void sqlite3async_shutdown();

void sqlite3async_run();

/*
** This function may only be called when the asynchronous IO VFS is 
** installed (after a call to sqlite3async_initialize()). It is used 
** to query or configure various parameters that affect the operation 
** of the asynchronous IO VFS. At present there are three parameters 
** supported:
**
**   * The "halt" parameter, which configures the circumstances under
**     which the sqlite3async_run() parameter is configured.
**
**   * The "delay" parameter. Setting the delay parameter to a non-zero
**     value causes the sqlite3async_run() function to sleep for the
**     configured number of milliseconds between each queued write 
**     operation.
**
**   * The "lockfiles" parameter. This parameter determines whether or 
**     not the asynchronous IO VFS locks the database files it operates
**     on. Disabling file locking can improve throughput.
**
** This function is always passed two arguments. When setting the value
** of a parameter, the first argument must be one of SQLITEASYNC_HALT,
** SQLITEASYNC_DELAY or SQLITEASYNC_LOCKFILES. The second argument must
** be passed the new value for the parameter as type "int".
**
** When querying the current value of a paramter, the first argument must
** be one of SQLITEASYNC_GET_HALT, GET_DELAY or GET_LOCKFILES. The second 
** argument to this function must be of type (int *). The current value
** of the queried parameter is copied to the memory pointed to by the
** second argument. For example:
**
**   int eCurrentHalt;
**   int eNewHalt = SQLITEASYNC_HALT_IDLE;
**
**   sqlite3async_control(SQLITEASYNC_HALT, eNewHalt);
**   sqlite3async_control(SQLITEASYNC_GET_HALT, &eCurrentHalt);
**   assert( eNewHalt==eCurrentHalt );
**
** See below for more detail on each configuration parameter.
**
** SQLITEASYNC_HALT:
**
**   This is used to set the value of the "halt" parameter. The second
**   argument must be one of the SQLITEASYNC_HALT_XXX symbols defined
**   below (either NEVER, IDLE and NOW).
**
**   If the parameter is set to NEVER, then calls to sqlite3async_run()
**   never return. This is the default setting. If the parameter is set
**   to IDLE, then calls to sqlite3async_run() return as soon as the
**   queue of pending write operations is empty. If the parameter is set
**   to NOW, then calls to sqlite3async_run() return as quickly as 
**   possible, without processing any pending write requests.
**
**   If an attempt is made to set this parameter to an integer value other
**   than SQLITEASYNC_HALT_NEVER, IDLE or NOW, then sqlite3async_control() 
**   returns SQLITE_MISUSE and the current value of the parameter is not 
**   modified.
**
**   Modifying the "halt" parameter affects calls to sqlite3async_run() 
**   made by other threads that are currently in progress.
**
** SQLITEASYNC_DELAY:
**
**   This is used to set the value of the "delay" parameter. If set to
**   a non-zero value, then after completing a pending write request, the
**   sqlite3async_run() function sleeps for the configured number of 
**   milliseconds.
**
**   If an attempt is made to set this parameter to a negative value,
**   sqlite3async_control() returns SQLITE_MISUSE and the current value
**   of the parameter is not modified.
**
**   Modifying the "delay" parameter affects calls to sqlite3async_run() 
**   made by other threads that are currently in progress.
**
** SQLITEASYNC_LOCKFILES:
**
**   This is used to set the value of the "lockfiles" parameter. This
**   parameter must be set to either 0 or 1. If set to 1, then the
**   asynchronous IO VFS uses the xLock() and xUnlock() methods of the
**   parent VFS to lock database files being read and/or written. If
**   the parameter is set to 0, then these locks are omitted.
**
**   This parameter may only be set when there are no open database
**   connections using the VFS and the queue of pending write requests
**   is empty. Attempting to set it when this is not true, or to set it 
**   to a value other than 0 or 1 causes sqlite3async_control() to return
**   SQLITE_MISUSE and the value of the parameter to remain unchanged.
**
**   If this parameter is set to zero, then it is only safe to access the
**   database via the asynchronous IO VFS from within a single process. If
**   while writing to the database via the asynchronous IO VFS the database
**   is also read or written from within another process, or via another
**   connection that does not use the asynchronous IO VFS within the same
**   process, the results are undefined (and may include crashes or database
**   corruption).
**
**   Alternatively, if this parameter is set to 1, then it is safe to access
**   the database from multiple connections within multiple processes using
**   either the asynchronous IO VFS or the parent VFS directly.
*/
int sqlite3async_control(int op, ...);

#define SQLITEASYNC_HALT          1
#define SQLITEASYNC_GET_HALT      2
#define SQLITEASYNC_DELAY         3
#define SQLITEASYNC_GET_DELAY     4
#define SQLITEASYNC_LOCKFILES     5
#define SQLITEASYNC_GET_LOCKFILES 6

#define SQLITEASYNC_HALT_NEVER 0       
#define SQLITEASYNC_HALT_NOW   1       
#define SQLITEASYNC_HALT_IDLE  2       

#ifdef __cplusplus
}  
#endif
#endif        

