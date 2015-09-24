// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_HISTORY_INDEX_RESTORE_OBSERVER_H_
#define CHROME_TEST_BASE_HISTORY_INDEX_RESTORE_OBSERVER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/history/in_memory_url_index.h"

class HistoryIndexRestoreObserver
    : public history::InMemoryURLIndex::RestoreCacheObserver {
 public:
  explicit HistoryIndexRestoreObserver(const base::Closure& task);
  virtual ~HistoryIndexRestoreObserver();

  bool succeeded() const { return succeeded_; }

  
  virtual void OnCacheRestoreFinished(bool success) OVERRIDE;

 private:
  base::Closure task_;
  bool succeeded_;

  DISALLOW_COPY_AND_ASSIGN(HistoryIndexRestoreObserver);
};

#endif  
