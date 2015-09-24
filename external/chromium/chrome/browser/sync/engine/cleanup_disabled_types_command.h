// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_CLEANUP_DISABLED_TYPES_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_CLEANUP_DISABLED_TYPES_COMMAND_H_
#pragma once

#include "chrome/browser/sync/engine/syncer_command.h"

namespace browser_sync {

class CleanupDisabledTypesCommand : public SyncerCommand {
 public:
  CleanupDisabledTypesCommand();
  virtual ~CleanupDisabledTypesCommand();

  
  virtual void ExecuteImpl(sessions::SyncSession* session);

 private:
  DISALLOW_COPY_AND_ASSIGN(CleanupDisabledTypesCommand);
};

}  

#endif  

