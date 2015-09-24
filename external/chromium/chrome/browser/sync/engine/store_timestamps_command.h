// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_STORE_TIMESTAMPS_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_STORE_TIMESTAMPS_COMMAND_H_
#pragma once

#include "chrome/browser/sync/engine/syncer_command.h"
#include "chrome/browser/sync/engine/syncer_types.h"

namespace browser_sync {

class StoreTimestampsCommand : public SyncerCommand {
 public:
  StoreTimestampsCommand();
  virtual ~StoreTimestampsCommand();

  
  virtual void ExecuteImpl(sessions::SyncSession* session);

 private:
  DISALLOW_COPY_AND_ASSIGN(StoreTimestampsCommand);
};

}  

#endif  
