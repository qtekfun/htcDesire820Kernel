// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_PROCESS_PROXY_PROCESS_OUTPUT_WATCHER_H_
#define CHROMEOS_PROCESS_PROXY_PROCESS_OUTPUT_WATCHER_H_

#include <string>

#include "base/callback.h"
#include "chromeos/chromeos_export.h"

namespace {

const int kReadBufferSize = 256;

}  

namespace chromeos {

enum ProcessOutputType {
  PROCESS_OUTPUT_TYPE_OUT,
  PROCESS_OUTPUT_TYPE_ERR,
  PROCESS_OUTPUT_TYPE_EXIT
};

typedef base::Callback<void(ProcessOutputType, const std::string&)>
      ProcessOutputCallback;

class CHROMEOS_EXPORT ProcessOutputWatcher {
 public:
  ProcessOutputWatcher(int out_fd, int stop_fd,
                       const ProcessOutputCallback& callback);

  
  void Start();

 private:
  
  ~ProcessOutputWatcher();

  // Listens to output from supplied fds. It guarantees data written to one fd
  // will be reported in order that it has been written (this is not true across
  
  void WatchProcessOutput();

  
  void VerifyFileDescriptor(int fd);

  
  void ReadFromFd(ProcessOutputType type, int* fd);

  
  void OnStop();

  char read_buffer_[kReadBufferSize];
  ssize_t read_buffer_size_;

  int out_fd_;
  int stop_fd_;
  int max_fd_;

  
  ProcessOutputCallback on_read_callback_;

  DISALLOW_COPY_AND_ASSIGN(ProcessOutputWatcher);
};

}  

#endif  
