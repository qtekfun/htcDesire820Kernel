// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_MOUNT_NODE_TTY_H_
#define LIBRARIES_NACL_IO_MOUNT_NODE_TTY_H_

#include <poll.h>
#include <pthread.h>

#include <deque>

#include "nacl_io/ioctl.h"
#include "nacl_io/mount_node_char.h"
#include "nacl_io/ostermios.h"


namespace nacl_io {

class MountNodeTty : public MountNodeCharDevice {
 public:
  explicit MountNodeTty(Mount* mount);

  virtual EventEmitter* GetEventEmitter();

  virtual Error VIoctl(int request, va_list args);

  virtual Error Read(const HandleAttr& attr,
                     void* buf,
                     size_t count,
                     int* out_bytes);

  virtual Error Write(const HandleAttr& attr,
                      const void* buf,
                      size_t count,
                      int* out_bytes);

  virtual Error Tcgetattr(struct termios* termios_p);
  virtual Error Tcsetattr(int optional_actions,
                          const struct termios *termios_p);

 private:
  ScopedEventEmitter emitter_;

  Error ProcessInput(struct tioc_nacl_input_string* message);
  Error Echo(const char* string, int count);
  void InitTermios();

  std::deque<char> input_buffer_;
  struct termios termios_;

  
  int rows_;
  
  int cols_;

  
  struct tioc_nacl_output output_handler_;
  
  
  sdk_util::SimpleLock output_lock_;
};

}

#endif
