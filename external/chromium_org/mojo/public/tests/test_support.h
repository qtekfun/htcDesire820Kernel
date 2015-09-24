// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_TESTS_TEST_SUPPORT_H_
#define MOJO_PUBLIC_TESTS_TEST_SUPPORT_H_

#include <string>

#include "base/callback.h"
#include "mojo/public/system/core_cpp.h"

namespace mojo {
namespace test {

struct MessagePipe {
  MessagePipe();
  ~MessagePipe();

  ScopedMessagePipeHandle handle_0;
  ScopedMessagePipeHandle handle_1;

  DISALLOW_COPY_AND_ASSIGN(MessagePipe);
};

bool WriteTextMessage(MessagePipeHandle handle, const std::string& text);
bool ReadTextMessage(MessagePipeHandle handle, std::string* text);

void IterateAndReportPerf(const char* test_name,
                          base::Callback<void()> single_iteration);

MojoResult WriteEmptyMessage(const MessagePipeHandle& handle);
MojoResult ReadEmptyMessage(const MessagePipeHandle& handle);

}  
}  

#endif  