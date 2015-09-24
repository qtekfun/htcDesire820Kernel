// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_PLATFORM_CHANNEL_H_
#define MOJO_SYSTEM_PLATFORM_CHANNEL_H_

#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/launch.h"
#include "mojo/system/platform_channel_handle.h"
#include "mojo/system/system_impl_export.h"

class CommandLine;

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT PlatformChannel {
 public:
  virtual ~PlatformChannel();

  
  PlatformChannelHandle PassHandle();

  bool is_valid() const { return handle_.is_valid(); }

 protected:
  PlatformChannel();

  PlatformChannelHandle* mutable_handle() { return &handle_; }

 private:
  PlatformChannelHandle handle_;

  DISALLOW_COPY_AND_ASSIGN(PlatformChannel);
};

class PlatformClientChannel;

class MOJO_SYSTEM_IMPL_EXPORT PlatformServerChannel : public PlatformChannel {
 public:
  virtual ~PlatformServerChannel() {}

  static scoped_ptr<PlatformServerChannel> Create(const std::string& name);

  
  
  virtual scoped_ptr<PlatformClientChannel> CreateClientChannel() = 0;

  
  
  
  
  virtual void GetDataNeededToPassClientChannelToChildProcess(
      CommandLine* command_line,
      base::FileHandleMappingVector* file_handle_mapping) const = 0;
  
  
  virtual void ChildProcessLaunched() = 0;

  const std::string& name() const { return name_; }

 protected:
  explicit PlatformServerChannel(const std::string& name);

 private:
  const std::string name_;

  DISALLOW_COPY_AND_ASSIGN(PlatformServerChannel);
};

class MOJO_SYSTEM_IMPL_EXPORT PlatformClientChannel : public PlatformChannel {
 public:
  virtual ~PlatformClientChannel() {}

  
  
  static scoped_ptr<PlatformClientChannel> CreateFromHandle(
      const PlatformChannelHandle& handle);

  
  
  
  static scoped_ptr<PlatformClientChannel> CreateFromParentProcess(
      const CommandLine& command_line);

 private:
  PlatformClientChannel() {}

  DISALLOW_COPY_AND_ASSIGN(PlatformClientChannel);
};

}  
}  

#endif  
