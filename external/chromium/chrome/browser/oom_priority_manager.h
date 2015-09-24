// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_OOM_PRIORITY_MANAGER_H_
#define CHROME_BROWSER_OOM_PRIORITY_MANAGER_H_

#include <list>

#include "base/timer.h"
#include "base/process.h"

namespace browser {

class OomPriorityManager {
 public:
  OomPriorityManager();
  ~OomPriorityManager();

 private:
  struct RendererStats {
    bool is_pinned;
    base::TimeTicks last_selected;
    size_t memory_used;
    base::ProcessHandle renderer_handle;
  };
  typedef std::list<RendererStats> StatsList;

  void StartTimer();
  void StopTimer();

  
  
  void AdjustOomPriorities();

  
  void DoAdjustOomPriorities(StatsList list);

  static bool CompareRendererStats(RendererStats first, RendererStats second);

  base::RepeatingTimer<OomPriorityManager> timer_;

  DISALLOW_COPY_AND_ASSIGN(OomPriorityManager);
};
}  

DISABLE_RUNNABLE_METHOD_REFCOUNT(browser::OomPriorityManager);

#endif  
