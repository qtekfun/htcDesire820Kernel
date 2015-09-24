// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_CHANGE_LISTENER_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_CHANGE_LISTENER_H_
#pragma once

#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/syncable/syncable.h"

namespace syncable {

class DirectoryChangeListener {
 public:
  virtual void HandleCalculateChangesChangeEventFromSyncApi(
      const OriginalEntries& originals,
      const WriterTag& writer,
      BaseTransaction* trans) = 0;
  virtual void HandleCalculateChangesChangeEventFromSyncer(
      const OriginalEntries& originals,
      const WriterTag& writer,
      BaseTransaction* trans) = 0;
  virtual ModelTypeBitSet HandleTransactionEndingChangeEvent(
      BaseTransaction* trans) = 0;
  virtual void HandleTransactionCompleteChangeEvent(
      const ModelTypeBitSet& models_with_changes) = 0;
 protected:
  virtual ~DirectoryChangeListener() {}
};

}  

#endif  
