// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_COMMAND_H_
#pragma once

#include "base/basictypes.h"

namespace browser_sync {

namespace sessions {
class SyncSession;
}


class SyncerCommand {
 public:
  SyncerCommand();
  virtual ~SyncerCommand();

  
  void Execute(sessions::SyncSession* session);

  
  virtual void ExecuteImpl(sessions::SyncSession* session) = 0;
 private:
  void SendNotifications(sessions::SyncSession* session);
  DISALLOW_COPY_AND_ASSIGN(SyncerCommand);
};

}  

#endif  
