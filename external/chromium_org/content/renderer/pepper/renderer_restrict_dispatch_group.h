// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_RESTRICT_DISPATCH_GROUP_H_
#define CONTENT_RENDERER_PEPPER_RESTRICT_DISPATCH_GROUP_H_

#include "ipc/ipc_sync_channel.h"

namespace content {

enum RendererRestrictDispatchGroup {
  kRendererRestrictDispatchGroup_None =
      IPC::SyncChannel::kRestrictDispatchGroup_None,
  kRendererRestrictDispatchGroup_Pepper,
};

}  

#endif  
