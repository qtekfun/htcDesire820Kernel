#ifndef IOLOOPER_H
#define IOLOOPER_H

#include <stdint.h>


typedef struct IoLooper  IoLooper;

IoLooper*  iolooper_new(void);
void       iolooper_free( IoLooper*  iol );
void       iolooper_reset( IoLooper*  iol );

void       iolooper_add_read( IoLooper*  iol, int  fd );
void       iolooper_add_write( IoLooper*  iol, int  fd );
void       iolooper_del_read( IoLooper*  iol, int  fd );
void       iolooper_del_write( IoLooper*  iol, int  fd );

enum {
    IOLOOPER_READ = (1<<0),
    IOLOOPER_WRITE = (1<<1),
};
void       iolooper_modify( IoLooper*  iol, int fd, int oldflags, int newflags);

int        iolooper_poll( IoLooper*  iol );
int        iolooper_wait( IoLooper*  iol, int64_t  duration );

int        iolooper_is_read( IoLooper*  iol, int  fd );
int        iolooper_is_write( IoLooper*  iol, int  fd );
int        iolooper_has_operations( IoLooper*  iol );
int64_t    iolooper_now(void);
int iolooper_wait_absolute(IoLooper* iol, int64_t deadline);

#endif 
