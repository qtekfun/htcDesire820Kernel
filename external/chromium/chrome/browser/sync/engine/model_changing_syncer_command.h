// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_MODEL_CHANGING_SYNCER_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_MODEL_CHANGING_SYNCER_COMMAND_H_
#pragma once

#include "chrome/browser/sync/engine/syncer_command.h"

namespace browser_sync {
namespace sessions {
class SyncSession;
}

class ModelChangingSyncerCommand : public SyncerCommand {
 public:
  ModelChangingSyncerCommand() : work_session_(NULL) { }
  virtual ~ModelChangingSyncerCommand() { }

  
  virtual void ExecuteImpl(sessions::SyncSession* session);

  
  void StartChangingModel() {
    ModelChangingExecuteImpl(work_session_);
  }

  
  
  
  
  
  
  virtual bool ModelNeutralExecuteImpl(sessions::SyncSession* session);

  
  
  
  
  virtual void ModelChangingExecuteImpl(sessions::SyncSession* session) = 0;

 private:
  
  
  
  
  sessions::SyncSession* work_session_;

  DISALLOW_COPY_AND_ASSIGN(ModelChangingSyncerCommand);
};

}  

#endif  
