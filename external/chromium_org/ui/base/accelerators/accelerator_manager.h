// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ACCELERATORS_ACCELERATOR_MANAGER_H_
#define UI_BASE_ACCELERATORS_ACCELERATOR_MANAGER_H_

#include <list>
#include <map>
#include <utility>

#include "base/basictypes.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/ui_export.h"
#include "ui/events/event_constants.h"

namespace ui {

class UI_EXPORT AcceleratorManager {
 public:
  enum HandlerPriority {
    kNormalPriority,
    kHighPriority,
  };

  AcceleratorManager();
  ~AcceleratorManager();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Register(const Accelerator& accelerator,
                HandlerPriority priority,
                AcceleratorTarget* target);

  
  void Unregister(const Accelerator& accelerator, AcceleratorTarget* target);

  
  void UnregisterAll(AcceleratorTarget* target);

  
  
  
  
  
  
  bool Process(const Accelerator& accelerator);

  
  
  
  AcceleratorTarget* GetCurrentTarget(const Accelerator& accelertor) const;

  
  bool HasPriorityHandler(const Accelerator& accelerator) const;

 private:
  
  typedef std::list<AcceleratorTarget*> AcceleratorTargetList;
  
  
  typedef std::pair<bool, AcceleratorTargetList> AcceleratorTargets;
  typedef std::map<Accelerator, AcceleratorTargets> AcceleratorMap;
  AcceleratorMap accelerators_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratorManager);
};

}  

#endif  
