/*
 * signalfd/eventfd compatibility
 *
 * Copyright IBM, Corp. 2008
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

#ifndef QEMU_COMPATFD_H
#define QEMU_COMPATFD_H

#include <signal.h>

struct qemu_signalfd_siginfo {
    uint32_t ssi_signo;   
    int32_t  ssi_errno;   
    int32_t  ssi_code;    
    uint32_t ssi_pid;     
    uint32_t ssi_uid;     
    int32_t  ssi_fd;      
    uint32_t ssi_tid;     
    uint32_t ssi_band;    
    uint32_t ssi_overrun; 
    uint32_t ssi_trapno;  
    int32_t  ssi_status;  
    int32_t  ssi_int;     
    uint64_t ssi_ptr;     
    uint64_t ssi_utime;   
    uint64_t ssi_stime;   
    uint64_t ssi_addr;    
    uint8_t  pad[48];     
};

int qemu_signalfd(const sigset_t *mask);

#endif
