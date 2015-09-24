/*
 *  Derived from Goldfish include/linux/signalfd.h
 *
 *  Copyright (C) 2007  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#ifndef _LINUX_SIGNALFD_PORTABLE_H
#define _LINUX_SIGNALFD_PORTABLE_H

#include <linux/types.h>
#include <fcntl.h>

#define SFD_CLOEXEC             O_CLOEXEC
#define SFD_NONBLOCK            O_NONBLOCK

#include "fcntl_portable.h"

#define SFD_CLOEXEC_PORTABLE    O_CLOEXEC_PORTABLE
#define SFD_NONBLOCK_PORTABLE   O_NONBLOCK_PORTABLE

struct signalfd_siginfo {
        __u32 ssi_signo;
        __s32 ssi_errno;
        __s32 ssi_code;
        __u32 ssi_pid;
        __u32 ssi_uid;
        __s32 ssi_fd;
        __u32 ssi_tid;
        __u32 ssi_band;
        __u32 ssi_overrun;
        __u32 ssi_trapno;
        __s32 ssi_status;
        __s32 ssi_int;
        __u64 ssi_ptr;
        __u64 ssi_utime;
        __u64 ssi_stime;
        __u64 ssi_addr;

        __u8 __pad[48];
};

#endif 

