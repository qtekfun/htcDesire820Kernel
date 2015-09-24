// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#define _GNU_SOURCE  

#include "process_util.h"

#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const int kMaxOomScore = 1000;
static const int kMaxOldOomScore = 15;

bool AdjustOOMScore(pid_t process, int score) {
  if (score < 0 || score > kMaxOomScore)
    return false;

  char oom_adj[27];  
                     
  snprintf(oom_adj, sizeof(oom_adj), "/proc/%" PRIdMAX, (intmax_t)process);

  const int dirfd = open(oom_adj, O_RDONLY | O_DIRECTORY);
  if (dirfd < 0)
    return false;

  struct stat statbuf;
  if (fstat(dirfd, &statbuf) < 0) {
    close(dirfd);
    return false;
  }
  if (getuid() != statbuf.st_uid) {
    close(dirfd);
    return false;
  }

  int fd = openat(dirfd, "oom_score_adj", O_WRONLY);
  if (fd < 0) {
    
    
    fd = openat(dirfd, "oom_adj", O_WRONLY);
    if (fd < 0) {
      
      return false;
    } else {
      
      
      
      score = score * kMaxOldOomScore / kMaxOomScore;
    }
  }
  close(dirfd);

  char buf[11];  
  snprintf(buf, sizeof(buf), "%d", score);
  size_t len = strlen(buf);

  ssize_t bytes_written = write(fd, buf, len);
  close(fd);
  return (bytes_written == len);
}
