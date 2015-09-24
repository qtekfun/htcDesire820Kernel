// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IPC_UTIL_H_
#define REMOTING_HOST_IPC_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ipc/ipc_platform_file.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif  

namespace base {
class SingleThreadTaskRunner;
}  

namespace IPC {
class ChannelProxy;
class Listener;
}  

namespace remoting {

bool CreateConnectedIpcChannel(
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    IPC::Listener* listener,
    IPC::PlatformFileForTransit* client_out,
    scoped_ptr<IPC::ChannelProxy>* server_out);

#if defined(OS_WIN)

bool CreateIpcChannel(
    const std::string& channel_name,
    const std::string& pipe_security_descriptor,
    base::win::ScopedHandle* pipe_out);

#endif  

} 

#endif  
