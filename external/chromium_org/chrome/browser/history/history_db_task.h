// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_DB_TASK_H_
#define CHROME_BROWSER_HISTORY_HISTORY_DB_TASK_H_

#include "base/memory/ref_counted.h"

namespace history {

class HistoryBackend;
class HistoryDatabase;

class HistoryDBTask : public base::RefCountedThreadSafe<HistoryDBTask> {
 public:
  
  
  
  
  
  virtual bool RunOnDBThread(HistoryBackend* backend, HistoryDatabase* db) = 0;

  
  
  
  virtual void DoneRunOnMainThread() = 0;

 protected:
  friend class base::RefCountedThreadSafe<HistoryDBTask>;

  virtual ~HistoryDBTask() {}
};

}  

#endif  
