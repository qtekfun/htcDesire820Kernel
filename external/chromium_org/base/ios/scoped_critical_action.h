// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_IOS_SCOPED_CRITICAL_ACTION_H_
#define BASE_IOS_SCOPED_CRITICAL_ACTION_H_

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"

namespace base {
namespace ios {

class ScopedCriticalAction {
 public:
  ScopedCriticalAction();
  ~ScopedCriticalAction();

 private:
  
  
  
  
  class Core : public base::RefCountedThreadSafe<Core> {
   public:
    Core();

    
    void EndBackgroundTask();

   private:
    friend base::RefCountedThreadSafe<Core>;
    ~Core();

    
    
    
    
    
    unsigned int background_task_id_;
    Lock background_task_id_lock_;

    DISALLOW_COPY_AND_ASSIGN(Core);
  };

  
  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCriticalAction);
};

}  
}  

#endif  
