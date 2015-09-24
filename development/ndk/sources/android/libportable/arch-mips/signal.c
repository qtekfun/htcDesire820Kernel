/*
 * Copyright 2012, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <portability.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <signal_portable.h>
#include <portability.h>
#include <stdio.h>
#include <errno.h>
#include <errno_portable.h>
#include <asm/unistd-portable.h>
#include <asm/unistd.h>
#include <signalfd_portable.h>
#include <filefd_portable.h>

#define PORTABLE_TAG "signal_portable"
#include <log_portable.h>


#if SIGBUS_PORTABLE == SIGBUS
#error Bad build environment
#endif

typedef void  (*sig3handler_t)(int, siginfo_t *, void *);

static volatile int signal_handler_mapping_enabled = 1;

extern int syscall(int, ...);


__hidden void signal_disable_mapping()
{
    ALOGV("%s(): signal_handler_mapping_enabled:%d = 0;", __func__,
                 signal_handler_mapping_enabled);

    signal_handler_mapping_enabled = 0;
}


__hidden char *map_portable_signum_to_name(int portable_signum)
{
    char *name;

    switch(portable_signum) {
    case 0:                     name = "SIG_0_PORTABLE:0";              break;
    case SIGHUP_PORTABLE:       name = "SIGHUP_PORTABLE:1";             break;
    case SIGINT_PORTABLE:       name = "SIGINT_PORTABLE:2";             break;
    case SIGQUIT_PORTABLE:      name = "SIGQUIT_PORTABLE:3";            break;
    case SIGILL_PORTABLE:       name = "SIGILL_PORTABLE:4";             break;
    case SIGTRAP_PORTABLE:      name = "SIGTRAP_PORTABLE:5";            break;
    case SIGABRT_PORTABLE:      name = "SIGABRT_PORTABLE:6";            break;
    case SIGBUS_PORTABLE:       name = "SIGBUS_PORTABLE:7";             break;
    case SIGFPE_PORTABLE:       name = "SIGFPE_PORTABLE:8";             break;
    case SIGKILL_PORTABLE:      name = "SIGKILL_PORTABLE:9";            break;
    case SIGUSR1_PORTABLE:      name = "SIGUSR1_PORTABLE:10";           break;
    case SIGSEGV_PORTABLE:      name = "SIGSEGV_PORTABLE:11";           break;
    case SIGUSR2_PORTABLE:      name = "SIGUSR2_PORTABLE:12";           break;
    case SIGPIPE_PORTABLE:      name = "SIGPIPE_PORTABLE:13";           break;
    case SIGALRM_PORTABLE:      name = "SIGALRM_PORTABLE:14";           break;
    case SIGTERM_PORTABLE:      name = "SIGTERM_PORTABLE:15";           break;
    case SIGSTKFLT_PORTABLE:    name = "SIGSTKFLT_PORTABLE:16";         break;
    case SIGCHLD_PORTABLE:      name = "SIGCHLD_PORTABLE:17";           break;
    case SIGCONT_PORTABLE:      name = "SIGCONT_PORTABLE:18";           break;
    case SIGSTOP_PORTABLE:      name = "SIGSTOP_PORTABLE:19";           break;
    case SIGTSTP_PORTABLE:      name = "SIGTSTP_PORTABLE:20";           break;
    case SIGTTIN_PORTABLE:      name = "SIGTTIN_PORTABLE:21";           break;
    case SIGTTOU_PORTABLE:      name = "SIGTTOU_PORTABLE:22";           break;
    case SIGURG_PORTABLE:       name = "SIGURG_PORTABLE:23";            break;
    case SIGXCPU_PORTABLE:      name = "SIGXCPU_PORTABLE:24";           break;
    case SIGXFSZ_PORTABLE:      name = "SIGXFSZ_PORTABLE:25";           break;
    case SIGVTALRM_PORTABLE:    name = "SIGVTALRM_PORTABLE:26";         break;
    case SIGPROF_PORTABLE:      name = "SIGPROF_PORTABLE:27";           break;
    case SIGWINCH_PORTABLE:     name = "SIGWINCH_PORTABLE:28";          break;
    case SIGIO_PORTABLE:        name = "SIGIO_PORTABLE:29";             break;
    case SIGPWR_PORTABLE:       name = "SIGPWR_PORTABLE:30";            break;
    case SIGSYS_PORTABLE:       name = "SIGSYS_PORTABLE:31";            break;
    case SIGRTMIN_PORTABLE:     name = "SIGRTMIN_PORTABLE:32";          break;

    case SIGRT_1_PORTABLE:      name = "SIGRT_1_PORTABLE:33";           break;
    case SIGRT_2_PORTABLE:      name = "SIGRT_2_PORTABLE:34";           break;
    case SIGRT_3_PORTABLE:      name = "SIGRT_3_PORTABLE:35";           break;
    case SIGRT_4_PORTABLE:      name = "SIGRT_4_PORTABLE:36";           break;
    case SIGRT_5_PORTABLE:      name = "SIGRT_5_PORTABLE:37";           break;
    case SIGRT_6_PORTABLE:      name = "SIGRT_6_PORTABLE:38";           break;
    case SIGRT_7_PORTABLE:      name = "SIGRT_7_PORTABLE:39";           break;
    case SIGRT_8_PORTABLE:      name = "SIGRT_8_PORTABLE:40";           break;
    case SIGRT_9_PORTABLE:      name = "SIGRT_9_PORTABLE:41";           break;
    case SIGRT_10_PORTABLE:     name = "SIGRT_10_PORTABLE:42";          break;
    case SIGRT_11_PORTABLE:     name = "SIGRT_11_PORTABLE:43";          break;
    case SIGRT_12_PORTABLE:     name = "SIGRT_12_PORTABLE:44";          break;
    case SIGRT_13_PORTABLE:     name = "SIGRT_13_PORTABLE:45";          break;
    case SIGRT_14_PORTABLE:     name = "SIGRT_14_PORTABLE:46";          break;
    case SIGRT_15_PORTABLE:     name = "SIGRT_15_PORTABLE:47";          break;
    case SIGRT_16_PORTABLE:     name = "SIGRT_16_PORTABLE:48";          break;
    case SIGRT_17_PORTABLE:     name = "SIGRT_17_PORTABLE:49";          break;
    case SIGRT_18_PORTABLE:     name = "SIGRT_18_PORTABLE:50";          break;
    case SIGRT_19_PORTABLE:     name = "SIGRT_19_PORTABLE:51";          break;
    case SIGRT_20_PORTABLE:     name = "SIGRT_20_PORTABLE:52";          break;
    case SIGRT_21_PORTABLE:     name = "SIGRT_21_PORTABLE:53";          break;
    case SIGRT_22_PORTABLE:     name = "SIGRT_22_PORTABLE:54";          break;
    case SIGRT_23_PORTABLE:     name = "SIGRT_23_PORTABLE:55";          break;
    case SIGRT_24_PORTABLE:     name = "SIGRT_24_PORTABLE:56";          break;
    case SIGRT_25_PORTABLE:     name = "SIGRT_25_PORTABLE:57";          break;
    case SIGRT_26_PORTABLE:     name = "SIGRT_26_PORTABLE:58";          break;
    case SIGRT_27_PORTABLE:     name = "SIGRT_27_PORTABLE:59";          break;
    case SIGRT_28_PORTABLE:     name = "SIGRT_28_PORTABLE:60";          break;
    case SIGRT_29_PORTABLE:     name = "SIGRT_29_PORTABLE:61";          break;
    case SIGRT_30_PORTABLE:     name = "SIGRT_30_PORTABLE:62";          break;
    case SIGRT_31_PORTABLE:     name = "SIGRT_31_PORTABLE:63";          break;
    case SIGRTMAX_PORTABLE:     name = "SIGRTMAX_PORTABLE:64";          break;

    default:                    name = "<<UNKNOWN>>";                   break;
    }
    return name;
}


__hidden char *map_mips_signum_to_name(int mips_signum)
{
    char *name;

    switch(mips_signum) {
    case 0:             name = "SIG_0:0";       break;
    case SIGHUP:        name = "SIGHUP:1";      break;
    case SIGINT:        name = "SIGINT:2";      break;
    case SIGQUIT:       name = "SIGQUIT:3";     break;
    case SIGILL:        name = "SIGILL:4";      break;
    case SIGTRAP:       name = "SIGTRAP:5";     break;
    case SIGIOT:        name = "SIGIOT:6";      break;
    case SIGEMT:        name = "SIGEMT:7";      break;
    case SIGFPE:        name = "SIGFPE:8";      break;
    case SIGKILL:       name = "SIGKILL:9";     break;
    case SIGBUS:        name = "SIGBUS:10";     break;
    case SIGSEGV:       name = "SIGSEGV:11";    break;
    case SIGSYS:        name = "SIGSYS:12";     break;
    case SIGPIPE:       name = "SIGPIPE:13";    break;
    case SIGALRM:       name = "SIGALRM:14";    break;
    case SIGTERM:       name = "SIGTERM:15";    break;
    case SIGUSR1:       name = "SIGUSR1:16";    break;
    case SIGUSR2:       name = "SIGUSR2:17";    break;
    case SIGCHLD:       name = "SIGCHLD:18";    break;
    case SIGPWR:        name = "SIGPWR:19";     break;
    case SIGWINCH:      name = "SIGWINCH:20";   break;
    case SIGURG:        name = "SIGURG:21";     break;
    case SIGIO:         name = "SIGIO:22";      break;
    case SIGSTOP:       name = "SIGSTOP:23";    break;
    case SIGTSTP:       name = "SIGTSTP:24";    break;
    case SIGCONT:       name = "SIGCONT:25";    break;
    case SIGTTIN:       name = "SIGTTIN:26";    break;
    case SIGTTOU:       name = "SIGTTOU:27";    break;
    case SIGVTALRM:     name = "SIGVTALRM:28";  break;
    case SIGPROF:       name = "SIGPROF:29";    break;
    case SIGXCPU:       name = "SIGXCPU:30";    break;
    case SIGXFSZ:       name = "SIGXFSZ:31";    break;

    case SIGRTMIN:      name = "SIGRTMIN:32";   break;
    case SIGRT_1:       name = "SIGRT_1:33";    break;
    case SIGRT_2:       name = "SIGRT_2:34";    break;
    case SIGRT_3:       name = "SIGRT_3:35";    break;
    case SIGRT_4:       name = "SIGRT_4:36";    break;
    case SIGRT_5:       name = "SIGRT_5:37";    break;
    case SIGRT_6:       name = "SIGRT_6:38";    break;
    case SIGRT_7:       name = "SIGRT_7:39";    break;
    case SIGRT_8:       name = "SIGRT_8:40";    break;
    case SIGRT_9:       name = "SIGRT_9:41";    break;
    case SIGRT_10:      name = "SIGRT_10:42";   break;
    case SIGRT_11:      name = "SIGRT_11:43";   break;
    case SIGRT_12:      name = "SIGRT_12:44";   break;
    case SIGRT_13:      name = "SIGRT_13:45";   break;
    case SIGRT_14:      name = "SIGRT_14:46";   break;
    case SIGRT_15:      name = "SIGRT_15:47";   break;
    case SIGRT_16:      name = "SIGRT_16:48";   break;
    case SIGRT_17:      name = "SIGRT_17:49";   break;
    case SIGRT_18:      name = "SIGRT_18:50";   break;
    case SIGRT_19:      name = "SIGRT_19:51";   break;
    case SIGRT_20:      name = "SIGRT_20:52";   break;
    case SIGRT_21:      name = "SIGRT_21:53";   break;
    case SIGRT_22:      name = "SIGRT_22:54";   break;
    case SIGRT_23:      name = "SIGRT_23:55";   break;
    case SIGRT_24:      name = "SIGRT_24:56";   break;
    case SIGRT_25:      name = "SIGRT_25:57";   break;
    case SIGRT_26:      name = "SIGRT_26:58";   break;
    case SIGRT_27:      name = "SIGRT_27:59";   break;
    case SIGRT_28:      name = "SIGRT_28:60";   break;
    case SIGRT_29:      name = "SIGRT_29:61";   break;
    case SIGRT_30:      name = "SIGRT_30:62";   break;
    case SIGRT_31:      name = "SIGRT_31:63";   break;
    case SIGRT_32:      name = "SIGRT_32:64";   break;

    

    case SIGRTMAX:      name = "SIGRTMAX:128";  break;
    default:            name = "<<UNKNOWN>>";   break;
    }
    return name;
}


__hidden int signum_pton(int portable_signum)
{
    int mips_signum = -1;

    switch(portable_signum) {

    case 0:                             
        return 0;

    case SIGHUP_PORTABLE:               
        return SIGHUP;

    case SIGINT_PORTABLE:               
        return SIGINT;

    case SIGQUIT_PORTABLE:              
        return SIGQUIT;

    case SIGILL_PORTABLE:               
        return SIGILL;

    case SIGTRAP_PORTABLE:              
        return SIGTRAP;

    case SIGABRT_PORTABLE:              
        return SIGABRT;

    case SIGBUS_PORTABLE:               
        return SIGBUS;

    case SIGFPE_PORTABLE:               
        return SIGFPE;

    case SIGKILL_PORTABLE:              
        return SIGKILL;

    case SIGUSR1_PORTABLE:              
        return SIGUSR1;

    case SIGSEGV_PORTABLE:              
        return SIGSEGV;

    case SIGUSR2_PORTABLE:              
        return SIGUSR2;

    case SIGPIPE_PORTABLE:              
        return SIGPIPE;

    case SIGALRM_PORTABLE:              
        return SIGALRM;

    case SIGTERM_PORTABLE:              
        return SIGTERM;

    case SIGSTKFLT_PORTABLE:            
        return SIGEMT;                  

    case SIGCHLD_PORTABLE:              
        return SIGCHLD;

    case SIGCONT_PORTABLE:              
        return SIGCONT;

    case SIGSTOP_PORTABLE:              
        return SIGSTOP;

    case SIGTSTP_PORTABLE:              
        return SIGTSTP;

    case SIGTTIN_PORTABLE:              
        return SIGTTIN;

    case SIGTTOU_PORTABLE:              
        return SIGTTOU;

    case SIGURG_PORTABLE:               
        return SIGURG;

    case SIGXCPU_PORTABLE:              
        return SIGXCPU;

    case SIGXFSZ_PORTABLE:              
        return SIGXFSZ;

    case SIGVTALRM_PORTABLE:            
        return SIGVTALRM;

    case SIGPROF_PORTABLE:              
        return SIGPROF;

    case SIGWINCH_PORTABLE:             
        return SIGWINCH;

    case SIGIO_PORTABLE:                
        return SIGIO;

    case SIGPWR_PORTABLE:               
        return SIGPWR;

    case SIGSYS_PORTABLE:               
        return SIGSYS;
    case SIGRTMIN_PORTABLE...SIGRTMAX_PORTABLE:         
        ASSERT(SIGRTMIN_PORTABLE == SIGRTMIN);
        ASSERT(SIGRTMAX_PORTABLE <= SIGRTMAX);
        return portable_signum;

    default:
        ALOGE("%s: switch default: NOTE portable_signum:%d Not supported. Just a Test?",
              __func__,                 portable_signum);
        if (portable_signum < 0) {
            mips_signum = portable_signum;
        } else if (portable_signum > NSIG_PORTABLE) {
            mips_signum = (portable_signum - NSIG_PORTABLE) +  NSIG;
        } else {
            ALOGE("%s: 0 < portable_signum:%d <= NSIG_PORTABLE:%d; Not supported, return(0);",
                 __func__, portable_signum,      NSIG_PORTABLE);

            mips_signum = 0;
        }
        break;
    }
    ALOGV("%s(portable_signum:%d): return(mips_signum:%d);", __func__,
              portable_signum,            mips_signum);

    return mips_signum;
}


__hidden int signum_ntop(int mips_signum)
{
    int portable_ssignum = -1;

    switch(mips_signum) {
    case 0:                             
        return 0;

    case SIGHUP:                        
        return SIGHUP_PORTABLE;

    case SIGINT:                        
        return SIGINT_PORTABLE;

    case SIGQUIT:                       
        return SIGQUIT_PORTABLE;

    case SIGILL:                        
        return SIGILL_PORTABLE;

    case SIGTRAP:                       
        return SIGTRAP_PORTABLE;

    case SIGABRT:                       
        return SIGABRT_PORTABLE;

    case SIGBUS:                        
        return SIGBUS_PORTABLE;

    case SIGFPE:                        
        return SIGFPE_PORTABLE;

    case SIGKILL:                       
        return SIGKILL_PORTABLE;

    case SIGUSR1:                       
        return SIGUSR1_PORTABLE;

    case SIGSEGV:                       
        return SIGSEGV_PORTABLE;

    case SIGUSR2:                       
        return SIGUSR2_PORTABLE;

    case SIGPIPE:                       
        return SIGPIPE_PORTABLE;

    case SIGALRM:                       
        return SIGALRM_PORTABLE;

    case SIGTERM:                       
        return SIGTERM_PORTABLE;

    case SIGEMT:                        
        return SIGSTKFLT_PORTABLE;      

    case SIGCHLD:                       
        return SIGCHLD_PORTABLE;

    case SIGCONT:                       
        return SIGCONT_PORTABLE;

    case SIGSTOP:                       
        return SIGSTOP_PORTABLE;

    case SIGTSTP:                       
        return SIGTSTP_PORTABLE;

    case SIGTTIN:                       
        return SIGTTIN_PORTABLE;

    case SIGTTOU:                       
        return SIGTTOU_PORTABLE;

    case SIGURG:                        
        return SIGURG_PORTABLE;

    case SIGXCPU:                       
        return SIGXCPU_PORTABLE;

    case SIGXFSZ:                       
        return SIGXFSZ_PORTABLE;

    case SIGVTALRM:                     
        return SIGVTALRM_PORTABLE;

    case SIGPROF:                       
        return SIGPROF_PORTABLE;

    case SIGWINCH:                      
        return SIGWINCH_PORTABLE;

    case SIGIO:                         
        return SIGIO_PORTABLE;

    case SIGPWR:                        
        return SIGPWR_PORTABLE;

    case SIGSYS:                        
        return SIGSYS_PORTABLE;

    case SIGRTMIN...SIGRTMAX_PORTABLE:              
        ASSERT(SIGRTMIN == SIGRTMIN_PORTABLE);
        ASSERT(SIGRTMAX >= SIGRTMAX_PORTABLE);
        return mips_signum;

    case (SIGRTMAX_PORTABLE+1)...SIGRTMAX:          
        ASSERT(SIGRTMIN == SIGRTMIN_PORTABLE);
        ASSERT(SIGRTMAX >= SIGRTMAX_PORTABLE);

        ALOGE("%s: mips_signum:%d Can't be mapped to a unique portable signal;", __func__,
                   mips_signum);

        ALOGE("%s: Mapping highest 63 Real Time Signals to the largest RT Portable SigNo.",
                __func__);

        return SIGRTMAX_PORTABLE;


    default:
        ALOGE("%s: switch default: mips_signum:%d Not supported! return(0);", __func__,
                                   mips_signum);
#if 0
        LOG_FATAL("%s: mips_signum:%d is not portable;", __func__, mips_signum);
#endif
        return 0;
    }
    return portable_ssignum;
}


static void siginfo_pton(siginfo_portable_t *portable_sip, siginfo_t *native_sip)
{

    ALOGV("%s(portable_sip:%p, native_sip:%p) {", __func__,
              portable_sip,    native_sip);

    ASSERT(sizeof(siginfo_portable_t) == sizeof(siginfo_t));


    *native_sip = *((siginfo_t *)portable_sip);
    native_sip->si_signo = 0;
    native_sip->si_code = portable_sip->si_code;
    native_sip->si_errno = errno_pton(portable_sip->si_errno);

    ALOGV("%s: return; }", __func__);
}


static void siginfo_ntop(siginfo_t *native_sip, siginfo_portable_t *portable_sip)
{

    ALOGV("%s(native_sip,:%p, portable_sip:%p) {", __func__,
              native_sip,     portable_sip);

    ASSERT(sizeof(siginfo_portable_t) == sizeof(siginfo_t));

    *portable_sip = *((siginfo_portable_t *)native_sip);

    portable_sip->si_signo = signum_ntop(native_sip->si_signo);
    portable_sip->si_code = native_sip->si_code;
    portable_sip->si_errno = errno_ntop(native_sip->si_errno);

    ALOGV("%s: return; }", __func__);
}


static sig3handler_portable_t mips_portable_sighandler[NSIG_PORTABLE + 1] = { NULL };

static void mips_sigaction_handler(int mips_signum, siginfo_t *sip, void *ucp)
{
    int portable_signum;
    char *portable_signame;
    char *mips_signame = map_mips_signum_to_name(mips_signum);
    sig3handler_portable_t portable_sighandler;
    siginfo_portable_t portable_si;
    siginfo_portable_t *portable_sip;

    ALOGV(" ");
    ALOGV("%s(mips_signum:%d:'%s', sip:%p, ucp:%p) {", __func__,
              mips_signum,
              mips_signame,        sip,    ucp);

    portable_signum = signum_ntop(mips_signum);
    portable_signame = map_portable_signum_to_name(portable_signum);
    portable_sighandler = mips_portable_sighandler[portable_signum];

    if (invalid_pointer(portable_sighandler)) {
        ALOGE("%s: invalid_pointer(portable_sighandler:%p); Likely about to Trap or Bus Error!",
                __func__,          portable_sighandler);

        ALOGE("%s: HINT: Likely best to use gdbserver and look at sigaction arguments.", __func__);
    }
    ASSERT(portable_sighandler != NULL);
    ASSERT(portable_sighandler != (sig3handler_portable_t) SIG_DFL);
    ASSERT(portable_sighandler != (sig3handler_portable_t) SIG_IGN);

    if (sip == NULL) {
        portable_sip = NULL;
    } else {
        
        portable_sip = &portable_si;
        siginfo_ntop(sip, portable_sip);
    }


    ALOGV("%s: Calling portable_sighandler:%p(portable_signum:%d, portable_sip:%p, ucp:%p);",
          __func__,    portable_sighandler,   portable_signum,    portable_sip,    ucp);

    portable_sighandler(portable_signum, portable_sip, ucp);

    ALOGV("%s: return; }", __func__);
}


static void mips_sighandler(int mips_signum)
{
    int portable_signum;
    char *portable_signame;
    char *mips_signame = map_mips_signum_to_name(mips_signum);
    sig3handler_portable_t portable_sighandler;

    ALOGV(" ");
    ALOGV("%s(mips_signum:%d:'%s') {", __func__, mips_signum, mips_signame);

    mips_sigaction_handler(mips_signum, NULL, NULL);

    ALOGV("%s: return; }", __func__);
}


static sighandler_t sighandler_pton(sighandler_portable_t portable_handler, int sigaction)
{
    sighandler_t mips_handler;

    ALOGV("%s(portable_handler:%p, sigaction:%d) {", __func__,
              portable_handler,    sigaction);

    switch((int) portable_handler) {
    case (int) SIG_DFL:
    case (int) SIG_IGN:
        mips_handler = portable_handler;
        break;

    default:    
        if (invalid_pointer(portable_handler)) {
            ALOGE("%s: invalid_pointer(portable_handler:%p)!", __func__, portable_handler);
            ALOGE("%s: HINT: Likely to cause a BUS Error ....", __func__);
            ALOGE("%s: HINT: ... when the signal handler is called!", __func__);
        }

        if (signal_handler_mapping_enabled) {
            if (sigaction)
                mips_handler = (sighandler_t) mips_sigaction_handler;
            else
                mips_handler = (sighandler_t) mips_sighandler;
        } else {
            mips_handler = portable_handler;        
        }
        break;
    }

    ALOGV("%s: return(mips_handler:%p); }", __func__, mips_handler);
    return mips_handler;
}


static sighandler_portable_t
do_signal_portable(int portable_signum, sighandler_portable_t portable_handler,
                   __sighandler_t (mips_signal_fn)(int, __sighandler_t))
{
    char *portable_signame = map_portable_signum_to_name(portable_signum);
    int mips_signum;
    sighandler_t mips_handler;
    sighandler_portable_t rv;
    sighandler_portable_t prev_portable_handler;

    ALOGV("%s(portable_signum:%d:%s, portable_handler:%p,  mips_signal_fn:%p) {", __func__,
              portable_signum,
              portable_signame,      portable_handler,     mips_signal_fn);

    mips_signum = signum_pton(portable_signum);

    if ((mips_signum <= 0) || (mips_signum > NSIG)) {
        mips_handler = sighandler_pton(portable_handler, 0);
        rv = mips_signal_fn(mips_signum, mips_handler);
    } else {
        prev_portable_handler = (sighandler_portable_t) mips_portable_sighandler[portable_signum];

        mips_handler = sighandler_pton(portable_handler, 0);
        if (mips_handler != portable_handler) {
            mips_portable_sighandler[portable_signum] = (sig3handler_portable_t) portable_handler;
        }
        rv = mips_signal_fn(mips_signum, mips_handler);

        if ((rv == (sighandler_portable_t) mips_sighandler) ||
            (rv == (sighandler_portable_t) mips_sigaction_handler)) {

            rv = (sighandler_t) prev_portable_handler;
        }
    }

    ALOGV("%s: return(rv:%p); }", __func__, rv);
    return rv;
}


sighandler_portable_t WRAP(signal)(int portable_signum, sighandler_portable_t handler)
{
    extern __sighandler_t REAL(bsd_signal)(int, __sighandler_t);
    sighandler_portable_t rv;

    ALOGV(" ");
    ALOGV("%s(portable_signum:%d, handler:%p) {", __func__,
              portable_signum,    handler);

    
    rv = do_signal_portable(portable_signum, handler, REAL(bsd_signal));

    ALOGV("%s: return(ret:%p); }", __func__, rv);
    return rv;
}


sighandler_portable_t WRAP(sysv_signal)(int portable_signum, sighandler_portable_t handler)
{
    extern __sighandler_t REAL(sysv_signal)(int, __sighandler_t);
    sighandler_portable_t rv;

    ALOGV(" ");
    ALOGV("%s(portable_signum:%d, handler:%p) {", __func__,
              portable_signum,    handler);

    
    rv = do_signal_portable(portable_signum, handler, REAL(sysv_signal));

    ALOGV("%s: return(ret:%p); }", __func__, rv);
    return rv;
}



sighandler_portable_t WRAP(bsd_signal)(int portable_signum, sighandler_portable_t handler)
{
    extern __sighandler_t REAL(bsd_signal)(int, __sighandler_t);
    sighandler_portable_t rv;

    ALOGV(" ");
    ALOGV("%s(portable_signum:%d, handler:%p) {", __func__,
              portable_signum,    handler);

    
    rv = do_signal_portable(portable_signum, handler, REAL(bsd_signal));

    ALOGV("%s: return(ret:%p); }", __func__, rv);
    return rv;
}


static int do_kill(int id, int portable_signum, int (*fn)(int, int))
{
    char *portable_signame = map_portable_signum_to_name(portable_signum);
    int mips_signum;
    int rv;

    ALOGV("%s(id:%d, portable_signum:%d:'%s', fn:%p) {", __func__,
              id,    portable_signum,
                     portable_signame,        fn);

    mips_signum = signum_pton(portable_signum);

    if ((portable_signum != 0) && (mips_signum == 0)) {
        rv = 0;
    } else {
        ALOGV("%s: Calling fn:%p(id:%d, mips_signum:%d);", __func__,
                           fn,   id,    mips_signum);

        rv =  fn(id, mips_signum);
    }
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(killpg)(int pgrp, int portable_signum)
{
    extern int REAL(killpg)(int pgrp, int sig);
    int rv;

    ALOGV(" ");
    ALOGV("%s(pgrp:%d, portable_signum:%d) {", __func__,
              pgrp,    portable_signum);

    rv = do_kill(pgrp, portable_signum, REAL(killpg));

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(kill)(pid_t pid, int portable_signum)
{
    extern int REAL(kill)(pid_t, int);
    int rv;

    ALOGV(" ");
    ALOGV("%s(pid:%d, portable_signum:%d) {", __func__,
              pid,    portable_signum);

    rv = do_kill(pid, portable_signum, REAL(kill));

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(tkill)(int tid, int portable_signum)
{
    extern int REAL(tkill)(int, int);
    int rv;

    ALOGV(" ");
    ALOGV("%s(tid:%d, portable_signum:%d) {", __func__,
              tid,    portable_signum);

    rv = do_kill(tid, portable_signum, REAL(tkill));

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


#if 0
int WRAP(tgkill)(int tgid, int tid, int portable_signum)
{
    extern int tgkill(int, int, int);
    char *portable_signame = map_portable_signum_to_name(portable_signum);
    int mips_signum;
    int rv;

    ALOGV("%s(tgid:%d, tid:%d, portable_signum:%d:'%s') {", __func__,
              tgid,    tid,    portable_signum, portable_signame);

    mips_signum = signum_pton(portable_signum);

    if ((portable_signum != 0) && (mips_signum == 0))
        rv = 0;
    else
        rv = REAL(tgkill)(tgid, tid, mips_signum);

    ALOGV("%s: return rv:%d; }", __func__, rv);
    return rv;
}
#endif


int WRAP(raise)(int portable_signum)
{
    char *portable_signame = map_portable_signum_to_name(portable_signum);
    int mips_signum = signum_pton(portable_signum);
    int rv;

    ALOGV("%s(portable_signum:%d:'%s') {", __func__, portable_signum, portable_signame);

    if ((portable_signum != 0) && (mips_signum == 0))
        rv = 0;
    else
        rv = REAL(raise)(mips_signum);

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


void sigset_pton(sigset_portable_t *portable_sigset, sigset_t *mips_sigset)
{
    int portable_signum;

    ASSERT(mips_sigset != NULL);

    ALOGV("%s(portable_sigset:%p, mips_sigset:%p) {", __func__,
              portable_sigset,    mips_sigset);

    sigemptyset(mips_sigset);
    if (invalid_pointer((void *)portable_sigset)) {
        ALOGE("%s: portable_sigset:%p is not valid; returning empty set.", __func__,
                   portable_sigset);
        goto done;
    }

    for(portable_signum = 1; portable_signum <= NSIG_PORTABLE; portable_signum++) {

        if (WRAP(sigismember)(portable_sigset, portable_signum)) {
            char *portable_signame = map_portable_signum_to_name(portable_signum);
            int mips_signum = signum_pton(portable_signum);
            char *mips_signame;

            if (mips_signum != 0) {
                int err;

                mips_signame = map_mips_signum_to_name(mips_signum);
                ALOGV("%s: sigaddset(mips_sigset:%p, mips_signum:%d:'%s');", __func__,
                                     mips_sigset,    mips_signum,
                                                     mips_signame);

                err = sigaddset(mips_sigset, mips_signum);
                if (err == -1) {
                    PERROR("sigaddset");
                }
            }
        }
    }

done:
    ALOGV("%s: return; }", __func__);
    return;
}


void
sigset_ntop(const sigset_t *const_mips_sigset, sigset_portable_t *portable_sigset)
{
    int mips_signum;
    sigset_t *mips_sigset = (sigset_t *) const_mips_sigset;

    ALOGV("%s(const_mips_sigset:%p, portable_sigset:%p) {", __func__,
              const_mips_sigset,    portable_sigset);

    ASSERT(mips_sigset != NULL);

    if (invalid_pointer((void *)portable_sigset)) {
        ALOGE("%s: portable_sigset:%p is not Valid; can't return sigset", __func__,
                   portable_sigset);
        goto done;
    }
    WRAP(sigemptyset)(portable_sigset);

    for(mips_signum = 1; mips_signum <= NSIG; mips_signum++) {
        if (sigismember(mips_sigset, mips_signum)) {
            int portable_signum = signum_ntop(mips_signum);

            if (portable_signum != 0)
                WRAP(sigaddset)(portable_sigset, portable_signum);
        }
    }

done:
    ALOGV("%s: return; }", __func__);
    return;
}


static int sigaction_flags_pton(int portable_flags)
{
    int mips_flags = 0;

    if (portable_flags & SA_NOCLDSTOP_PORTABLE) {
        mips_flags |= SA_NOCLDSTOP;
    }
    if (portable_flags & SA_NOCLDWAIT_PORTABLE) {
        mips_flags |= SA_NOCLDWAIT;
    }
    if (portable_flags & SA_SIGINFO_PORTABLE) {
        mips_flags |= SA_SIGINFO;
    }
    if (portable_flags & SA_THIRTYTWO_PORTABLE) {
        ALOGV("%s: SA_THIRTYTWO_PORTABLE isn't SUPPORTED.", __func__);
    }
    if (portable_flags & SA_RESTORER_PORTABLE) {
        mips_flags |= SA_RESTORER;
    }
    if (portable_flags & SA_ONSTACK_PORTABLE) {
        mips_flags |= SA_ONSTACK;
    }
    if (portable_flags & SA_RESTART_PORTABLE) {
        mips_flags |= SA_RESTART;
    }
    if (portable_flags & SA_NODEFER_PORTABLE) {
        mips_flags |= SA_NODEFER;
    }
    if (portable_flags & SA_RESETHAND_PORTABLE) {
        mips_flags |= SA_RESETHAND;
    }

    ALOGV("%s(portable_flags:0x%x) return(mips_flags:0x%x);", __func__,
              portable_flags,             mips_flags);

    return mips_flags;
}


int sigaction_flags_ntop(int mips_flags)
{
    int portable_flags = 0;

    if (mips_flags & SA_NOCLDSTOP)      portable_flags |= SA_NOCLDSTOP_PORTABLE;
    if (mips_flags & SA_NOCLDWAIT)      portable_flags |= SA_NOCLDWAIT_PORTABLE;
    if (mips_flags & SA_SIGINFO)        portable_flags |= SA_SIGINFO_PORTABLE;
#ifdef SA_THIRTYTWO
    if (mips_flags & SA_THIRTYTWO)      portable_flags |= SA_THIRTYTWO_PORTABLE;
#endif
    if (mips_flags & SA_RESTORER)       portable_flags |= SA_RESTORER_PORTABLE;
    if (mips_flags & SA_ONSTACK)        portable_flags |= SA_ONSTACK_PORTABLE;
    if (mips_flags & SA_RESTART)        portable_flags |= SA_RESTART_PORTABLE;
    if (mips_flags & SA_NODEFER)        portable_flags |= SA_NODEFER_PORTABLE;
    if (mips_flags & SA_RESETHAND)      portable_flags |= SA_RESETHAND_PORTABLE;

    ALOGV("%s(mips_flags:0x%x) return(portable_flags:0x%x);", __func__,
              mips_flags,             portable_flags);

    return portable_flags;
}


static int do_sigaction_portable(int portable_signum, const struct sigaction_portable *act,
                                 struct sigaction_portable *oldact, sigaction_fn fn,
                                 rt_sigaction_fn rt_fn)
{
    int mips_signum;
    char *mips_signame;
    struct sigaction mips_act;
    struct sigaction *mips_act_ptr;
    struct sigaction mips_oldact;
    sighandler_t mips_handler;
    sighandler_portable_t portable_handler;
    sig3handler_portable_t prev_portable_handler;
    char *portable_signame = map_portable_signum_to_name(portable_signum);
    int rv;

    ALOGV("%s(portable_signum:%d:'%s', act:%p, oldact:%p, fn:%p, rt_fn:%p) {", __func__,
              portable_signum,
              portable_signame,        act,    oldact,    fn,    rt_fn);

    mips_signum = signum_pton(portable_signum);
    mips_signame = map_mips_signum_to_name(mips_signum);

    if ((portable_signum != 0) && (mips_signum == 0)) {
        rv = 0;
        goto done;
    }
    if (portable_signum > 0 && portable_signum <= NSIG_PORTABLE)
        prev_portable_handler = mips_portable_sighandler[portable_signum];
    else
        prev_portable_handler = NULL;

    memset(&mips_act, 0, sizeof(mips_act));

    if (invalid_pointer((void *)act)) {
        mips_act_ptr = (struct sigaction *)act;
    } else {
        sigset_pton(((sigset_portable_t *)&act->sa_mask),
                                    ((sigset_t *) &mips_act.sa_mask));

        mips_act.sa_flags = sigaction_flags_pton(act->sa_flags);

        if (mips_act.sa_flags & SA_SIGINFO) {
            portable_handler = (sighandler_portable_t) act->sa_sigaction_portable;
            if ((portable_signum <= 0) || (portable_signum > NSIG_PORTABLE)) {
                mips_act.sa_sigaction = (sig3handler_t) portable_handler;
            } else {
                mips_handler = sighandler_pton(portable_handler, 1);
                if (mips_handler != portable_handler) {
                    mips_portable_sighandler[portable_signum] =
                                                       (sig3handler_portable_t) portable_handler;
                }
                mips_act.sa_sigaction = (sig3handler_t) mips_handler;
            }
        } else {
            portable_handler = act->sa_handler_portable;
            if ((portable_signum <= 0) || (portable_signum > NSIG_PORTABLE)) {
                mips_act.sa_handler = (sighandler_t) portable_handler;
            } else {
                mips_handler = sighandler_pton(portable_handler, 1);
                if (mips_handler != portable_handler) {
                    mips_portable_sighandler[portable_signum] =
                                                       (sig3handler_portable_t) portable_handler;
                }
                mips_act.sa_handler = mips_handler;
            }
        }
        mips_act_ptr = &mips_act;
    }

    if (fn != NULL) {
        ASSERT(rt_fn == NULL);
        rv = fn(mips_signum, mips_act_ptr, &mips_oldact);
    } else {
        ASSERT(rt_fn != NULL);
        rv = rt_fn(mips_signum, mips_act_ptr, &mips_oldact, sizeof(sigset_t));
    }

    if (rv == 0 && oldact) {
        if (mips_oldact.sa_sigaction == (__sigaction_handler_portable_t) mips_sigaction_handler ||
            mips_oldact.sa_sigaction == (__sigaction_handler_portable_t) mips_sighandler) {

            oldact->sa_sigaction_portable =
                                        (__sigaction_handler_portable_t) prev_portable_handler;
        } else {
            oldact->sa_sigaction_portable =
                                        (__sigaction_handler_portable_t) mips_oldact.sa_sigaction;
        }
        sigset_ntop((sigset_t *) &(mips_oldact.sa_mask),
                                    (sigset_portable_t *) &(oldact->sa_mask));

        oldact->sa_flags = sigaction_flags_ntop(mips_oldact.sa_flags);
        oldact->sa_restorer = NULL;
    }

done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(sigaction)(int portable_signum, const struct sigaction_portable *act,
                       struct sigaction_portable *oldact)
{
    extern int REAL(sigaction)(int, const struct sigaction *, struct sigaction *);
    int rv;

    ALOGV(" ");
    ALOGV("%s(portable_signum:%d, act:%p, oldact:%p) {", __func__,
              portable_signum,    act,    oldact);

    rv = do_sigaction_portable(portable_signum, act, oldact, REAL(sigaction), NULL);

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


__hidden int do_signalfd4_portable(int fd, const sigset_portable_t *portable_sigmask,
                                   int portable_sigsetsize, int portable_flags)
{
    sigset_t native_sigmask;
    int native_sigsetsize = sizeof(native_sigmask);
    int native_flags = 0;
    int rv;

    ALOGV("%s(fd:%d, portable_sigmask:%p, portable_sigsetsize:%d, portable_flags:0x%x) {",
    __func__, fd,    portable_sigmask,    portable_sigsetsize,    portable_flags);

    sigset_pton((sigset_portable_t *)portable_sigmask, &native_sigmask);

    if (portable_flags & SFD_NONBLOCK_PORTABLE) {
        native_flags |= SFD_NONBLOCK;
    }
    if (portable_flags & SFD_CLOEXEC_PORTABLE) {
        native_flags |= SFD_CLOEXEC;
    }
    rv = syscall(__NR_signalfd4, fd, &native_sigmask, native_sigsetsize, native_flags);

    if (rv >= 0) {
        if (native_flags & SFD_CLOEXEC) {
            filefd_CLOEXEC_enabled(rv);
        }

        filefd_opened(rv, SIGNAL_FD_TYPE);
    }

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


#if 0
int WRAP(signalfd)(int fd, const sigset_portable_t *portable_sigmask, int portable_flags)
{
    int portable_sigsetsize = sizeof(sigset_portable_t);
    int rv;

    ALOGV("%s(fd:%d, portable_sigmask:%p, portable_flags:0x%x) {", __func__,
              fd,    portable_sigmask,    portable_flags);

    rv = do_signalfd4_portable(fd, portable_sigsetsize, portable_sigmask, portable_flags);

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}
#endif


__hidden int read_signalfd_mapper(int fd, void *buf, size_t count)
{
    int rv;

    ALOGV("%s(fd:%d, buf:0x%p, count:%d) {", __func__,
              fd,    buf,      count);

    rv = read(fd, buf, count);
    if (rv > 0) {
        int siginfos = rv/sizeof(struct signalfd_siginfo);
        struct signalfd_siginfo *si = (struct signalfd_siginfo *) buf;
        int i;

        
        for (i = 0; i < siginfos; i++, si++) {
            int ssi_signo;

            ssi_signo = si->ssi_signo;
            si->ssi_signo = signum_ntop(si->ssi_signo);
            ALOGV("%s: si->ssi_signo:%d = signum_ntop(si->ssi_signo:%d); i:%d", __func__,
                       si->ssi_signo,                     ssi_signo,     i);

            si->ssi_errno = errno_ntop(si->ssi_errno);

            if (si->ssi_status > 0 && si->ssi_status <= NSIG) {
                si->ssi_status = signum_ntop(si->ssi_status);
            }

        }
    }

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}

int WRAP(sigsuspend)(const sigset_portable_t *portable_sigmask)
{
    int rv;
    sigset_t mips_sigmask;

    ALOGV("%s(portable_sigmask:%p) {", __func__, portable_sigmask);

    if (invalid_pointer((void *)portable_sigmask)) {
        *REAL(__errno)() = EFAULT;
        rv = -1;
    } else {
        sigset_pton((sigset_portable_t *)portable_sigmask, &mips_sigmask);
        rv = REAL(sigsuspend)(&mips_sigmask);
    }

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(sigpending)(sigset_portable_t *portable_sigset)
{
    int rv;
    sigset_t mips_sigset;

    ALOGV("%s(portable_sigset:%p) {", __func__,
              portable_sigset);

    if (invalid_pointer((void *)portable_sigset)) {
        *REAL(__errno)() = EFAULT;
        rv = -1;
    } else {
        rv = REAL(sigpending)(&mips_sigset);
        sigset_ntop(&mips_sigset, portable_sigset);
    }

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(sigwait)(const sigset_portable_t *portable_sigset, int *ptr_to_portable_sig)
{
    int rv;
    sigset_t mips_sigset;
    int mips_sig;
    int portable_sig;

    ALOGV("%s(portable_sigset:%p, ptr_to_portable_sig:%p) {", __func__,
              portable_sigset,    ptr_to_portable_sig);

    if (invalid_pointer((void *)portable_sigset)) {
        *REAL(__errno)() = EFAULT;
        rv = -1;
    } else {
        sigset_pton((sigset_portable_t *)portable_sigset, &mips_sigset);

        rv = REAL(sigwait)(&mips_sigset, &mips_sig);

        portable_sig = signum_ntop(mips_sig);
        *ptr_to_portable_sig = portable_sig;
    }
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(siginterrupt)(int portable_signum, int flag)

{
    int rv;
    int mips_signum;

    ALOGV("%s(portable_signum:%d, flag:0x%x) {", __func__,
              portable_signum,    flag);

    mips_signum = signum_pton(portable_signum);

    if ((portable_signum != 0) && (mips_signum == 0)) {
        rv = 0;
    } else {
        rv = REAL(siginterrupt)(mips_signum, flag);
    }
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


__hidden int do_sigmask(int portable_how, const sigset_portable_t *portable_sigset,
                        sigset_portable_t *portable_oldset, sigmask_fn fn,
                        rt_sigmask_fn rt_fn)
{
    int rv;
    int how;
    char *how_name;
    sigset_t mips_sigset, *mips_sigset_p;
    sigset_t mips_oldset, *mips_oldset_p;

    ALOGV("%s(portable_how:%d, portable_sigset:%p, portable_oldset:%p, fn:%p, rt_fn:%p) {",
    __func__, portable_how,    portable_sigset,    portable_oldset,    fn,    rt_fn);

    switch(portable_how) {
    case SIG_BLOCK_PORTABLE:    how = SIG_BLOCK;        how_name = "SIG_BLOCK";         break;
    case SIG_UNBLOCK_PORTABLE:  how = SIG_UNBLOCK;      how_name = "SIG_UNBLOCK";       break;
    case SIG_SETMASK_PORTABLE:  how = SIG_SETMASK;      how_name = "SIG_SETMASK";       break;

    default:
        ALOGE("%s: portable_how:%d NOT SUPPORTED!", __func__, portable_how);
        how = -1;
        break;
    }

    if (invalid_pointer((void *)portable_sigset)) {
        mips_sigset_p = (sigset_t *) portable_sigset;
    } else {
        mips_sigset_p = &mips_sigset;
        memset(mips_sigset_p, 0, sizeof(mips_sigset));
        sigemptyset(mips_sigset_p);
        sigset_pton((sigset_portable_t *)portable_sigset, &mips_sigset);
    }

    if (invalid_pointer((void *)portable_oldset)) {
        mips_oldset_p = (sigset_t *) portable_oldset;
    } else {
        mips_oldset_p = &mips_oldset;
        memset(mips_oldset_p, 0, sizeof(mips_oldset));
        sigemptyset(mips_oldset_p);
    }

    if (fn != NULL) {
        ASSERT(rt_fn == NULL);
        rv = fn(how, mips_sigset_p, mips_oldset_p);
    } else {
        ASSERT(rt_fn != NULL);
        rv = rt_fn(how, mips_sigset_p, mips_oldset_p, sizeof(sigset_t));
    }

    if (rv == 0 && !invalid_pointer(portable_oldset)) {
        
        sigset_ntop(mips_oldset_p, portable_oldset);
    }

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(sigprocmask)(int portable_how, const sigset_portable_t *portable_sigset,
                         sigset_portable_t *portable_oldset)
{
    extern int REAL(sigprocmask)(int, const sigset_t *, sigset_t *);
    int rv;

    ALOGV(" ");
    ALOGV("%s(portable_how:%d, portable_sigset:%p, portable_oldset:%p) {", __func__,
              portable_how,    portable_sigset,    portable_oldset);

    rv = do_sigmask(portable_how, portable_sigset, portable_oldset, REAL(sigprocmask), NULL);

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


int WRAP(__rt_sigaction)(int portable_signum, const struct sigaction_portable *act,
                            struct sigaction_portable *oldact, size_t sigsetsize)
{
    extern int REAL(__rt_sigaction)(int , const struct sigaction *, struct sigaction *, size_t);
    int rv;

    ALOGV(" ");
    ALOGV("%s(portable_signum:%d, act:%p, oldset:%p, sigsetsize:%d) {", __func__,
              portable_signum,    act,    oldact,    sigsetsize);

    
    if (sigsetsize != (2* sizeof(long))) {
        *REAL(__errno)() = EINVAL;
        rv = -1;
        goto done;
    }
    rv = do_sigaction_portable(portable_signum, act, oldact, NULL, REAL(__rt_sigaction));

done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}

int WRAP(__rt_sigprocmask)(int portable_how,
                              const sigset_portable_t *portable_sigset,
                              sigset_portable_t *portable_oldset,
                              size_t sigsetsize)
{
    extern int REAL(__rt_sigprocmask)(int, const sigset_t *, sigset_t *, size_t);
    int rv;

    ALOGV(" ");
    ALOGV("%s(portable_how:%d, portable_sigset:%p, portable_oldset:%p, sigsetsize:%d) {",
    __func__, portable_how,    portable_sigset,    portable_oldset,    sigsetsize);

    
    if (sigsetsize != (2* sizeof(long))) {
        *REAL(__errno)() = EINVAL;
        rv = -1;
        goto done;
    }
    rv = do_sigmask(portable_how, portable_sigset, portable_oldset, NULL, REAL(__rt_sigprocmask));

 done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);

    return rv;
}


int WRAP(__rt_sigtimedwait)(const sigset_portable_t *portable_sigset,
                               siginfo_portable_t *portable_siginfo,
                               const struct timespec *timeout,
                               size_t portable_sigsetsize)
{
    extern int REAL(__rt_sigtimedwait)(const sigset_t *, siginfo_t *, const struct timespec *, size_t);

    sigset_t native_sigset_struct;
    sigset_t *native_sigset = &native_sigset_struct;
    siginfo_t native_siginfo_struct;
    siginfo_t *native_siginfo;
    int rv;

    ALOGV(" ");
    ALOGV("%s(portable_sigset:%p, portable_siginfo:%p, timeout:%p, portable_sigsetsize:%d) {",
    __func__, portable_sigset,    portable_siginfo,    timeout,    portable_sigsetsize);

    
    if (portable_sigsetsize != (2* sizeof(long))) {
        *REAL(__errno)() = EINVAL;
        rv = -1;
        goto done;
    }
    if (portable_sigset == NULL) {
        native_sigset = NULL;
    } else {
        sigset_pton((sigset_portable_t *)portable_sigset, native_sigset);
    }
    if (portable_siginfo == NULL) {
        native_siginfo = NULL;
    } else {
        native_siginfo = &native_siginfo_struct;
    }
    rv = REAL(__rt_sigtimedwait)(native_sigset, native_siginfo, timeout, sizeof(sigset_t));
    if (rv == 0 && native_siginfo != NULL) {
        
        siginfo_ntop(native_siginfo, portable_siginfo);
    }

done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}


#ifdef  __NR_rt_sigqueueinfo

#if 0
int WRAP(sigqueue)(pid_t pid, int portable_sig, const union sigval value)
{
    siginfo_t native_siginfo;
    siginfo_t *native_sip;
    siginfo_portable_t portable_siginfo;
    siginfo_portable_t *portable_sip;
    int native_sig;
    int rv;

    ALOGV(" ");
    ALOGV("%s(pid:%d, portable_sig:%d, value:%p) {", __func__,
              pid,    portable_sig,    value.sival_ptr);

    native_sig = signum_pton(portable_sig);
    native_sip = &native_siginfo;

    portable_sip = &portable_siginfo;
    portable_sip->si_signo = 0;                 
    portable_sip->si_code = SI_QUEUE;
    portable_sip->si_pid = getpid();            
    portable_sip->si_uid = getuid();            
    portable_sip->si_value = value;             

    siginfo_pton(portable_sip, native_sip);

    ALOGV("%s: calling syscall(__NR_rt_sigqueueinfo:%d, pid:%d, native_sig:%d, native_sip:%p);",
           __func__,           __NR_rt_sigqueueinfo,    pid,    native_sig,    native_sip);

    rv = syscall(__NR_rt_sigqueueinfo, pid, native_sig, native_sip);

    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}
#endif


int WRAP(rt_sigqueueinfo)(pid_t pid, int portable_sig, siginfo_portable_t *portable_sip)
{
    int native_sig;
    siginfo_t native_siginfo, *native_sip;
    int rv;

    ALOGV(" ");
    ALOGV("%s(pid:%d, portable_sig:%d, portable_sip:%p) {", __func__,
              pid,    portable_sig,    portable_sip);

    native_sig = signum_pton(portable_sig);

    if ((portable_sig != 0) && (native_sig == 0)) {
        rv = 0;
        goto done;
    }

    if (portable_sip != NULL) {
        native_sip = &native_siginfo;
        siginfo_pton(portable_sip, native_sip);
    } else {
        native_sip = NULL;
    }
    rv = syscall(__NR_rt_sigqueueinfo, pid, native_sig, native_sip);

done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}
#endif 


#ifdef __NR_rt_tgsigqueueinfo
int WRAP(rt_tgsigqueueinfo)(pid_t tgid, pid_t pid, int portable_sig,
                               siginfo_portable_t *portable_sip)
{
    siginfo_t native_siginfo, *native_sip;
    int native_sig;
    int rv;

    ALOGV(" ");
    ALOGV("%s(tgid:%d, pid:%d, portable_sig:%d, portable_sip:%p) {", __func__,
              tgid,    pid,    portable_sig,    portable_sip);

    native_sig = signum_pton(portable_sig);

    if ((portable_sig != 0) && (native_sig == 0)) {
        rv = 0;
        goto done;
    }
    if (portable_sip != NULL) {
        native_sip = &native_siginfo;
        siginfo_pton(portable_sip, native_sip);
    } else {
        native_sip = NULL;
    }
    rv = syscall(__NR_rt_tgsigqueueinfo, pid, native_sig, native_sip);

done:
    ALOGV("%s: return(rv:%d); }", __func__, rv);
    return rv;
}
#endif 


int WRAP(sigaltstack)(const portable_stack_t *ss, portable_stack_t *oss)
{
    int rv;
    stack_t new_stack, *mips_ss;
    stack_t old_stack, *mips_oss;

    ALOGV(" ");
    ALOGV("%s(ss:%p, oss:%p) {", __func__, ss, oss);

    if (ss == NULL) {
        mips_ss = NULL;
    } else {
        if (invalid_pointer((void *)ss)) {
            ALOGE("%s: invalid_pointer(ss:%p): Let kernel set proper errno and set return value.",
                   __func__,           ss);

            mips_ss = (stack_t *) ss;
        } else {
            memset(&new_stack, 0, sizeof(stack_t));
            new_stack.ss_sp = ss->ss_sp;
            new_stack.ss_flags = ss->ss_flags;
            new_stack.ss_size = ss->ss_size;
            mips_ss = &new_stack;
        }
    }
    if (oss == NULL) {
        mips_oss = NULL;
    } else {
        if (invalid_pointer((void *)oss)) {
            ALOGE("%s: invalid_pointer(oss:%p): Let kernel set proper errno and return value.",
                   __func__,           oss);

            mips_oss = (stack_t *)oss;
        } else {
            memset(&old_stack, 0, sizeof(stack_t));
            mips_oss = &old_stack;
        }
    }

    rv = REAL(sigaltstack)(mips_ss, mips_oss);

    if (!invalid_pointer(oss)) {
        oss->ss_sp = old_stack.ss_sp;
        oss->ss_flags = old_stack.ss_flags;
        oss->ss_size = old_stack.ss_size;
    }
    ALOGV("%s: return(rv:%d); }", __func__, rv);

    return rv;
}
