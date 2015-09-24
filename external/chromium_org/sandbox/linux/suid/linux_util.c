// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#define _GNU_SOURCE  
#include "linux_util.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char kSocketLinkPrefix[] = "socket:[";

static bool ProcPathGetInodeAt(ino_t* inode_out, int base_dir_fd,
                               const char* path) {
  
  if (!inode_out || !path || *path == '/')
    return false;
  char buf[256];
  const ssize_t n = readlinkat(base_dir_fd, path, buf, sizeof(buf) - 1);
  if (n < 0)
    return false;
  buf[n] = 0;

  if (memcmp(kSocketLinkPrefix, buf, sizeof(kSocketLinkPrefix) - 1))
    return false;

  char *endptr = NULL;
  errno = 0;
  const unsigned long long int inode_ull =
      strtoull(buf + sizeof(kSocketLinkPrefix) - 1, &endptr, 10);
  if (inode_ull == ULLONG_MAX || !endptr || *endptr != ']' || errno != 0)
    return false;

  *inode_out = inode_ull;
  return true;
}

static DIR* opendirat(int base_dir_fd, const char* name) {
  
  if (base_dir_fd < 0 || !name || *name == '/')
    return NULL;
  int new_dir_fd = openat(base_dir_fd, name, O_RDONLY | O_DIRECTORY);
  if (new_dir_fd < 0)
    return NULL;

  return fdopendir(new_dir_fd);
}

bool FindProcessHoldingSocket(pid_t* pid_out, ino_t socket_inode) {
  bool already_found = false;

  DIR* proc = opendir("/proc");
  if (!proc)
    return false;

  const uid_t uid = getuid();
  struct dirent* dent;
  while ((dent = readdir(proc))) {
    char *endptr = NULL;
    errno = 0;
    const unsigned long int pid_ul = strtoul(dent->d_name, &endptr, 10);
    if (pid_ul == ULONG_MAX || !endptr || *endptr || errno != 0)
      continue;

    
    
    
    
    
    int proc_pid_fd = -1;
    {
      char buf[256];
      struct stat statbuf;
      snprintf(buf, sizeof(buf), "/proc/%lu", pid_ul);
      proc_pid_fd = open(buf, O_RDONLY | O_DIRECTORY);
      if (proc_pid_fd < 0)
        continue;
      if (fstat(proc_pid_fd, &statbuf) < 0 || uid != statbuf.st_uid) {
        close(proc_pid_fd);
        continue;
      }
    }

    DIR* fd = opendirat(proc_pid_fd, "fd");
    if (!fd) {
      close(proc_pid_fd);
      continue;
    }

    while ((dent = readdir(fd))) {
      char buf[256];
      int printed = snprintf(buf, sizeof(buf), "fd/%s", dent->d_name);
      if (printed < 0 || printed >= (int)(sizeof(buf) - 1)) {
        continue;
      }

      ino_t fd_inode;
      if (ProcPathGetInodeAt(&fd_inode, proc_pid_fd, buf)) {
        if (fd_inode == socket_inode) {
          if (already_found) {
            closedir(fd);
            close(proc_pid_fd);
            closedir(proc);
            return false;
          }

          already_found = true;
          *pid_out = pid_ul;
          break;
        }
      }
    }
    closedir(fd);
    close(proc_pid_fd);
  }
  closedir(proc);

  return already_found;
}
