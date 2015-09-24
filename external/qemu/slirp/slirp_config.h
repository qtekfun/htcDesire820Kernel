
#undef EMULATE_TALK

#undef PROBE_CONN

#define DO_KEEPALIVE 0

#define MAX_INTERFACES 1
#define MAX_PPP_INTERFACES 1

#undef USE_TMPSOCKET

#undef DO_CFSETSPEED

#undef FULL_BOLT

#undef USE_LOWCPU

#ifndef __STDC__
#define NO_PROTOTYPES
#endif


#undef DUMMY_PPP

#define HAVE_UNISTD_H

#define HAVE_STDLIB_H

#undef HAVE_SYS_IOCTL_H
#ifndef _WIN32
#define HAVE_SYS_IOCTL_H
#endif

#undef HAVE_SYS_FILIO_H
#ifdef __APPLE__
#define HAVE_SYS_FILIO_H
#endif

#define HAVE_STRERROR

#define HAVE_STRDUP

#define TIME_WITH_SYS_TIME 0
#undef HAVE_SYS_TIME_H

#undef HAVE_SYS_BITYPES_H


#undef BAD_SPRINTF

#undef HAVE_READV

#undef DECLARE_IOVEC
#ifdef _WIN32
#define DECLARE_IOVEC
#endif

#undef DECLARE_SPRINTF

#undef HAVE_SYS_WAIT_H

#undef HAVE_SYS_SELECT_H
#ifndef _WIN32
#define HAVE_SYS_SELECT_H
#endif

#define HAVE_STRING_H

#undef HAVE_ARPA_INET_H
#ifndef _WIN32
#define HAVE_ARPA_INET_H
#endif

#undef HAVE_SYS_SIGNAL_H

#undef HAVE_SYS_STROPTS_H



#undef NO_PROTOTYPES

#undef NEED_TYPEDEFS
#ifdef __sun__
#define NEED_TYPEDEFS
#endif

#define SIZEOF_CHAR 1

#define SIZEOF_SHORT 2

#define SIZEOF_INT 4

#define SIZEOF_CHAR_P (HOST_LONG_BITS / 8)

#undef HAVE_RANDOM

#undef HAVE_SRANDOM

#undef HAVE_INET_ATON
#ifndef _WIN32
#define HAVE_INET_ATON
#endif

#undef HAVE_SETENV

#define HAVE_INDEX

#undef HAVE_BCMP

#undef HAVE_DRAND48

#define HAVE_MEMMOVE

#define HAVE_GETHOSTID

#undef NO_UNIX_SOCKETS
#ifdef _WIN32
#define NO_UNIX_SOCKETS
#endif

#undef GETTIMEOFDAY_ONE_ARG

#undef HAVE_REVOKE

#undef HAVE_GRANTPT

#undef HAVE_FCHMOD

#undef HAVE_SYS_TYPES32_H
