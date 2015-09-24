// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "common/sandbox.h"

#define _GNU_SOURCE
#include <asm/unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <unistd.h>

#include "linux_util.h"
#include "process_util.h"
#include "common/suid_unsafe_environment_variables.h"

#if !defined(CLONE_NEWPID)
#define CLONE_NEWPID 0x20000000
#endif
#if !defined(CLONE_NEWNET)
#define CLONE_NEWNET 0x40000000
#endif

static bool DropRoot();

#define HANDLE_EINTR(x) TEMP_FAILURE_RETRY(x)

static void FatalError(const char *msg, ...)
    __attribute__((noreturn, format(printf, 1, 2)));

static void FatalError(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);

  vfprintf(stderr, msg, ap);
  fprintf(stderr, ": %s\n", strerror(errno));
  fflush(stderr);
  va_end(ap);
  _exit(1);
}

static void ExitWithErrorSignalHandler(int signal) {
  const char msg[] = "\nThe setuid sandbox got signaled, exiting.\n";
  if (-1 == write(2, msg, sizeof(msg) - 1)) {
    
  }

  _exit(1);
}

#define SAFE_DIR "/proc/self/fdinfo"
#define SAFE_DIR2 "/proc/self/fd"

static bool SpawnChrootHelper() {
  int sv[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
    perror("socketpair");
    return false;
  }

  char *safedir = NULL;
  struct stat sdir_stat;
  if (!stat(SAFE_DIR, &sdir_stat) && S_ISDIR(sdir_stat.st_mode))
    safedir = SAFE_DIR;
  else
    if (!stat(SAFE_DIR2, &sdir_stat) && S_ISDIR(sdir_stat.st_mode))
      safedir = SAFE_DIR2;
    else {
      fprintf(stderr, "Could not find %s\n", SAFE_DIR2);
      return false;
    }

  const pid_t pid = syscall(
      __NR_clone, CLONE_FS | SIGCHLD, 0, 0, 0);

  if (pid == -1) {
    perror("clone");
    close(sv[0]);
    close(sv[1]);
    return false;
  }

  if (pid == 0) {
    
    
    

    const struct rlimit nofile = {0, 0};
    if (setrlimit(RLIMIT_NOFILE, &nofile))
      FatalError("Setting RLIMIT_NOFILE");

    if (close(sv[1]))
      FatalError("close");

    
    char msg;
    ssize_t bytes;
    do {
      bytes = read(sv[0], &msg, 1);
    } while (bytes == -1 && errno == EINTR);

    if (bytes == 0)
      _exit(0);
    if (bytes != 1)
      FatalError("read");

    
    if (msg != kMsgChrootMe)
      FatalError("Unknown message from sandboxed process");

    
    if (chdir(safedir))
      FatalError("Cannot chdir into /proc/ directory");

    if (chroot(safedir))
      FatalError("Cannot chroot into /proc/ directory");

    if (chdir("/"))
      FatalError("Cannot chdir to / after chroot");

    const char reply = kMsgChrootSuccessful;
    do {
      bytes = write(sv[0], &reply, 1);
    } while (bytes == -1 && errno == EINTR);

    if (bytes != 1)
      FatalError("Writing reply");

    _exit(0);
    
    
    
    
    
  }

  if (close(sv[0])) {
    close(sv[1]);
    perror("close");
    return false;
  }

  
  
  char desc_str[64];
  int printed = snprintf(desc_str, sizeof(desc_str), "%u", sv[1]);
  if (printed < 0 || printed >= (int)sizeof(desc_str)) {
    fprintf(stderr, "Failed to snprintf\n");
    return false;
  }

  if (setenv(kSandboxDescriptorEnvironmentVarName, desc_str, 1)) {
    perror("setenv");
    close(sv[1]);
    return false;
  }

  
  char helper_pid_str[64];
  printed = snprintf(helper_pid_str, sizeof(helper_pid_str), "%u", pid);
  if (printed < 0 || printed >= (int)sizeof(helper_pid_str)) {
    fprintf(stderr, "Failed to snprintf\n");
    return false;
  }

  if (setenv(kSandboxHelperPidEnvironmentVarName, helper_pid_str, 1)) {
    perror("setenv");
    close(sv[1]);
    return false;
  }

  return true;
}

static void WaitForChildAndExit(pid_t child_pid) {
  int exit_code = -1;
  siginfo_t reaped_child_info;

  
  
  
  
  
  if (signal(SIGABRT, ExitWithErrorSignalHandler) == SIG_ERR) {
    FatalError("Failed to change signal handler");
  }

  int wait_ret =
    HANDLE_EINTR(waitid(P_PID, child_pid, &reaped_child_info, WEXITED));

  if (!wait_ret && reaped_child_info.si_pid == child_pid) {
    if (reaped_child_info.si_code == CLD_EXITED) {
      exit_code = reaped_child_info.si_status;
    } else {
      
      exit_code = 0;
    }
  }
  _exit(exit_code);
}

static bool MoveToNewNamespaces() {
  
  const int kCloneExtraFlags[] = {
    CLONE_NEWPID | CLONE_NEWNET,
    CLONE_NEWPID,
  };

  
  
  int sync_fds[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sync_fds)) {
    FatalError("Failed to create a socketpair");
  }

  for (size_t i = 0;
       i < sizeof(kCloneExtraFlags) / sizeof(kCloneExtraFlags[0]);
       i++) {
    pid_t pid = syscall(__NR_clone, SIGCHLD | kCloneExtraFlags[i], 0, 0, 0);

    if (pid > 0) {
      if (!DropRoot()) {
        FatalError("Could not drop privileges");
      } else {
        if (close(sync_fds[0]) || shutdown(sync_fds[1], SHUT_RD))
          FatalError("Could not close socketpair");
        
        
        if (close(kZygoteIdFd))
          FatalError("close");
        
        if (HANDLE_EINTR(send(sync_fds[1], "C", 1, MSG_NOSIGNAL)) != 1)
          FatalError("send");
        if (close(sync_fds[1]))
          FatalError("close");
        
        
        WaitForChildAndExit(pid);
      }
      
      FatalError("Not reached");
    }

    if (pid == 0) {
      if (close(sync_fds[1]) || shutdown(sync_fds[0], SHUT_WR))
        FatalError("Could not close socketpair");

      
      
      char should_continue;
      if (HANDLE_EINTR(read(sync_fds[0], &should_continue, 1)) != 1)
        FatalError("Read on socketpair");
      if (close(sync_fds[0]))
        FatalError("close");

      if (kCloneExtraFlags[i] & CLONE_NEWPID) {
        setenv(kSandboxPIDNSEnvironmentVarName, "", 1 );
      } else {
        unsetenv(kSandboxPIDNSEnvironmentVarName);
      }

      if (kCloneExtraFlags[i] & CLONE_NEWNET) {
        setenv(kSandboxNETNSEnvironmentVarName, "", 1 );
      } else {
        unsetenv(kSandboxNETNSEnvironmentVarName);
      }

      break;
    }

    if (errno != EINVAL) {
      perror("Failed to move to new PID namespace");
      return false;
    }
  }

  
  return true;
}

static bool DropRoot() {
  if (prctl(PR_SET_DUMPABLE, 0, 0, 0, 0)) {
    perror("prctl(PR_SET_DUMPABLE)");
    return false;
  }

  if (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0)) {
    perror("Still dumpable after prctl(PR_SET_DUMPABLE)");
    return false;
  }

  gid_t rgid, egid, sgid;
  if (getresgid(&rgid, &egid, &sgid)) {
    perror("getresgid");
    return false;
  }

  if (setresgid(rgid, rgid, rgid)) {
    perror("setresgid");
    return false;
  }

  uid_t ruid, euid, suid;
  if (getresuid(&ruid, &euid, &suid)) {
    perror("getresuid");
    return false;
  }

  if (setresuid(ruid, ruid, ruid)) {
    perror("setresuid");
    return false;
  }

  return true;
}

static bool SetupChildEnvironment() {
  unsigned i;

  
  
  
  
  

  for (i = 0; kSUIDUnsafeEnvironmentVariables[i]; ++i) {
    const char* const envvar = kSUIDUnsafeEnvironmentVariables[i];
    char* const saved_envvar = SandboxSavedEnvironmentVariable(envvar);
    if (!saved_envvar)
      return false;

    const char* const value = getenv(saved_envvar);
    if (value) {
      setenv(envvar, value, 1 );
      unsetenv(saved_envvar);
    }

    free(saved_envvar);
  }

  return true;
}

bool CheckAndExportApiVersion() {
  
  
  long api_number = -1;
  char *api_string = getenv(kSandboxEnvironmentApiRequest);
  if (!api_string) {
    api_number = 0;
  } else {
    errno = 0;
    char* endptr = NULL;
    api_number = strtol(api_string, &endptr, 10);
    if (!endptr || *endptr || errno != 0)
      return false;
  }

  
  if (api_number != kSUIDSandboxApiNumber) {
    fprintf(stderr, "The setuid sandbox provides API version %ld, "
      "but you need %ld\n"
      "Please read "
      "https://code.google.com/p/chromium/wiki/LinuxSUIDSandboxDevelopment."
      "\n\n",
      kSUIDSandboxApiNumber,
      api_number);
  }

  
  
  char version_string[64];
  snprintf(version_string, sizeof(version_string), "%ld",
           kSUIDSandboxApiNumber);
  if (setenv(kSandboxEnvironmentApiProvides, version_string, 1)) {
    perror("setenv");
    return false;
  }

  return true;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    if (argc <= 0) {
      return 1;
    }

    fprintf(stderr, "Usage: %s <renderer process> <args...>\n", argv[0]);
    return 1;
  }

  
  if (argc == 2 && 0 == strcmp(argv[1], kSuidSandboxGetApiSwitch)) {
    printf("%ld\n", kSUIDSandboxApiNumber);
    return 0;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  if (argc == 3 && (0 == strcmp(argv[1], kFindInodeSwitch))) {
    pid_t pid;
    char* endptr = NULL;
    errno = 0;
    ino_t inode = strtoull(argv[2], &endptr, 10);
    if (inode == ULLONG_MAX || !endptr || *endptr || errno != 0)
      return 1;
    if (!FindProcessHoldingSocket(&pid, inode))
      return 1;
    printf("%d\n", pid);
    return 0;
  }
  
  
  if (argc == 4 && (0 == strcmp(argv[1], kAdjustOOMScoreSwitch))) {
    char* endptr = NULL;
    long score;
    errno = 0;
    unsigned long pid_ul = strtoul(argv[2], &endptr, 10);
    if (pid_ul == ULONG_MAX || !endptr || *endptr || errno != 0)
      return 1;
    pid_t pid = pid_ul;
    endptr = NULL;
    errno = 0;
    score = strtol(argv[3], &endptr, 10);
    if (score == LONG_MAX || score == LONG_MIN ||
        !endptr || *endptr || errno != 0)
      return 1;
    return AdjustOOMScore(pid, score);
  }

  
  if (!CheckAndExportApiVersion()) {
    return 1;
  }

  if (!MoveToNewNamespaces())
    return 1;
  if (!SpawnChrootHelper())
    return 1;
  if (!DropRoot())
    return 1;
  if (!SetupChildEnvironment())
    return 1;

  execv(argv[1], &argv[1]);
  FatalError("execv failed");

  return 1;
}
