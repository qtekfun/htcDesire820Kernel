// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_TIMED_TASK_HELPER_H_
#define WEBKIT_BROWSER_FILEAPI_TIMED_TASK_HELPER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "webkit/browser/webkit_storage_browser_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace fileapi {

class WEBKIT_STORAGE_BROWSER_EXPORT TimedTaskHelper {
 public:
  explicit TimedTaskHelper(base::SequencedTaskRunner* task_runner);
  ~TimedTaskHelper();

  bool IsRunning() const;
  void Start(const tracked_objects::Location& posted_from,
             base::TimeDelta delay,
             const base::Closure& user_task);
  void Reset();

 private:
  struct Tracker;
  static void Fired(scoped_ptr<Tracker> tracker);

  void OnFired(scoped_ptr<Tracker> tracker);
  void PostDelayedTask(scoped_ptr<Tracker> tracker, base::TimeDelta delay);

  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  tracked_objects::Location posted_from_;
  base::TimeDelta delay_;
  base::Closure user_task_;

  base::TimeTicks desired_run_time_;

  
  Tracker* tracker_;

  DISALLOW_COPY_AND_ASSIGN(TimedTaskHelper);
};

}  

#endif  
