// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_VERIFY_UPDATES_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_VERIFY_UPDATES_COMMAND_H_
#pragma once

#include "base/basictypes.h"

#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/engine/model_changing_syncer_command.h"
#include "chrome/browser/sync/engine/syncproto.h"
#include "chrome/browser/sync/engine/syncer_types.h"

namespace syncable {
class WriteTransaction;
}

namespace browser_sync {

class VerifyUpdatesCommand : public ModelChangingSyncerCommand {
 public:
  VerifyUpdatesCommand();
  virtual ~VerifyUpdatesCommand();

  
  virtual void ModelChangingExecuteImpl(sessions::SyncSession* session);

 private:
  struct VerifyUpdateResult {
    VerifyResult value;
    ModelSafeGroup placement;
  };
  VerifyUpdateResult VerifyUpdate(syncable::WriteTransaction* trans,
                                  const SyncEntity& entry,
                                  const ModelSafeRoutingInfo& routes);
  DISALLOW_COPY_AND_ASSIGN(VerifyUpdatesCommand);
};

}  

#endif  
