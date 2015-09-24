
#ifndef _SYS_SIGNAL_H
#define _SYS_SIGNAL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "_ansi.h"
#include <sys/features.h>
#include <sys/types.h>


typedef unsigned long sigset_t;

#if defined(__rtems__) || defined(__native_client__)

#if defined(_POSIX_REALTIME_SIGNALS)


#define SIGEV_NONE   1  
                        
#define SIGEV_SIGNAL 2  
                        
                        
#define SIGEV_THREAD 3  
                        


union sigval {
  int    sival_int;    
  void  *sival_ptr;    
};

struct sigevent {
  int              sigev_notify;               
  int              sigev_signo;                
  union sigval     sigev_value;                

#if defined(_POSIX_THREADS)
  void           (*sigev_notify_function)( union sigval );
                                               
  pthread_attr_t  *sigev_notify_attributes;    
#endif
};


#define SI_USER    1    
#define SI_QUEUE   2    
#define SI_TIMER   3    
#define SI_ASYNCIO 4    
#define SI_MESGQ   5    

typedef struct {
  int          si_signo;    
  int          si_code;     
  union sigval si_value;    
} siginfo_t;
#endif


#define SA_NOCLDSTOP 1   
#define SA_SIGINFO   2   
                         


typedef void (*_sig_func_ptr)(int);

struct sigaction {
  int         sa_flags;       
  sigset_t    sa_mask;        
                              
                              
  union {
    _sig_func_ptr _handler;  
#if defined(_POSIX_REALTIME_SIGNALS)
    void      (*_sigaction)( int, siginfo_t *, void * );
#endif
  } _signal_handlers;
};

#define sa_handler    _signal_handlers._handler
#if defined(_POSIX_REALTIME_SIGNALS)
#define sa_sigaction  _signal_handlers._sigaction
#endif

#elif defined(__CYGWIN__)
#include <cygwin/signal.h>
#else
#define SA_NOCLDSTOP 1  

typedef void (*_sig_func_ptr)(int);

struct sigaction 
{
	_sig_func_ptr sa_handler;
	sigset_t sa_mask;
	int sa_flags;
};
#endif 

#define SIG_SETMASK 0	
#define SIG_BLOCK 1	
#define SIG_UNBLOCK 2	

#define sigaddset(what,sig) (*(what) |= (1<<(sig)), 0)
#define sigdelset(what,sig) (*(what) &= ~(1<<(sig)), 0)
#define sigemptyset(what)   (*(what) = 0, 0)
#define sigfillset(what)    (*(what) = ~(0), 0)
#define sigismember(what,sig) (((*(what)) & (1<<(sig))) != 0)

int _EXFUN(sigprocmask, (int how, const sigset_t *set, sigset_t *oset));

#if defined(_POSIX_THREADS)
int _EXFUN(pthread_sigmask, (int how, const sigset_t *set, sigset_t *oset));
#endif

#if defined(__CYGWIN__) || defined(__rtems__) || defined (__native_client__)
#undef sigaddset
#undef sigdelset
#undef sigemptyset
#undef sigfillset
#undef sigismember

int _EXFUN(kill, (pid_t, int));
int _EXFUN(killpg, (pid_t, int));
int _EXFUN(sigaction, (int, const struct sigaction *, struct sigaction *));
int _EXFUN(sigaddset, (sigset_t *, const int));
int _EXFUN(sigdelset, (sigset_t *, const int));
int _EXFUN(sigismember, (const sigset_t *, int));
int _EXFUN(sigfillset, (sigset_t *));
int _EXFUN(sigemptyset, (sigset_t *));
int _EXFUN(sigpending, (sigset_t *));
int _EXFUN(sigsuspend, (const sigset_t *));
int _EXFUN(sigpause, (int));

#if defined(_POSIX_THREADS)
#ifdef __CYGWIN__
#  ifndef _CYGWIN_TYPES_H
#    error You need the winsup sources or a cygwin installation to compile the cygwin version of newlib.
#  endif
#endif
int _EXFUN(pthread_kill, (pthread_t thread, int sig));
#endif

#if defined(_POSIX_REALTIME_SIGNALS)


int _EXFUN(sigwaitinfo, (const sigset_t *set, siginfo_t *info));
int _EXFUN(sigtimedwait,
  (const sigset_t *set, siginfo_t *info, const struct timespec  *timeout)
);
int _EXFUN(sigwait, (const sigset_t *set, int *sig));

int _EXFUN(sigqueue, (pid_t pid, int signo, const union sigval value));

#endif 

#endif 


#if defined(___AM29K__)
#define SIGABRT 1
#define SIGFPE 1
#define SIGILL 1
#define SIGINT 1
#define SIGSEGV 1
#define SIGTERM 1
#define SIGHUP 1
#define SIGQUIT 1
#define NSIG 2
#elif defined(__GO32__)
#define SIGINT  1
#define SIGKILL 2
#define SIGPIPE 3
#define SIGFPE  4
#define SIGHUP  5
#define SIGTERM 6
#define SIGSEGV 7
#define SIGTSTP 8
#define SIGQUIT 9
#define SIGTRAP 10
#define SIGILL  11
#define SIGEMT  12
#define SIGALRM 13
#define SIGBUS  14
#define SIGLOST 15
#define SIGSTOP 16
#define SIGABRT 17
#define SIGUSR1	18
#define SIGUSR2	19
#define NSIG    20
#elif !defined(SIGTRAP)
#define	SIGHUP	1	
#define	SIGINT	2	
#define	SIGQUIT	3	
#define	SIGILL	4	
#define	SIGTRAP	5	
#define	SIGIOT	6	
#define	SIGABRT 6	
#define	SIGEMT	7	
#define	SIGFPE	8	
#define	SIGKILL	9	
#define	SIGBUS	10	
#define	SIGSEGV	11	
#define	SIGSYS	12	
#define	SIGPIPE	13	
#define	SIGALRM	14	
#define	SIGTERM	15	

#if defined(__rtems__)
#define	SIGURG	16	
#define	SIGSTOP	17	
#define	SIGTSTP	18	
#define	SIGCONT	19	
#define	SIGCHLD	20	
#define	SIGCLD	20	
#define	SIGTTIN	21	
#define	SIGTTOU	22	
#define	SIGIO	23	
#define	SIGPOLL	SIGIO	
#define	SIGWINCH 24	
#define	SIGUSR1 25	
#define	SIGUSR2 26	

#define SIGRTMIN 27
#define SIGRTMAX 31
#define __SIGFIRSTNOTRT SIGHUP
#define __SIGLASTNOTRT  SIGUSR2

#define NSIG	32      

#elif defined(__svr4__)
#define	SIGUSR1	16
#define SIGUSR2	17
#define SIGCLD	18
#define	SIGPWR	19
#define SIGWINCH 20
#define	SIGPOLL	22	
#define	SIGSTOP	23	
#define	SIGTSTP	24	
#define	SIGCONT	25	
#define	SIGTTIN	26	
#define	SIGTTOU	27	
#define NSIG	28	
#else
#define	SIGURG	16	
#define	SIGSTOP	17	
#define	SIGTSTP	18	
#define	SIGCONT	19	
#define	SIGCHLD	20	
#define	SIGCLD	20	
#define	SIGTTIN	21	
#define	SIGTTOU	22	
#define	SIGIO	23	
#define	SIGPOLL	SIGIO	
#define	SIGXCPU	24	
#define	SIGXFSZ	25	
#define	SIGVTALRM 26	
#define	SIGPROF	27	
#define	SIGWINCH 28	
#define	SIGLOST 29	
#define	SIGUSR1 30	
#define	SIGUSR2 31	
#define NSIG	32      
#endif
#endif

#ifdef __cplusplus
}
#endif

#ifndef _SIGNAL_H_
#include <signal.h>
#endif
#endif 
