// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_CLEAR_DATA_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_CLEAR_DATA_COMMAND_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/engine/syncer_command.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace sync_pb {
class EntitySpecifics;
}

namespace browser_sync {

class ClearDataCommand : public SyncerCommand {
 public:
  ClearDataCommand();
  virtual ~ClearDataCommand();

  
  virtual void ExecuteImpl(sessions::SyncSession* session);

 private:
  DISALLOW_COPY_AND_ASSIGN(ClearDataCommand);
};

}  

#endif  

