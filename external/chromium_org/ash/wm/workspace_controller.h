// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WORKSPACE_CONTROLLER_H_
#define ASH_WM_WORKSPACE_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/wm/workspace/workspace_types.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class ShelfLayoutManager;
class WorkspaceControllerTestHelper;
class WorkspaceEventHandler;
class WorkspaceLayoutManager;

class ASH_EXPORT WorkspaceController {
 public:
  explicit WorkspaceController(aura::Window* viewport);
  virtual ~WorkspaceController();

  
  WorkspaceWindowState GetWindowState() const;

  void SetShelf(ShelfLayoutManager* shelf);

  
  void DoInitialAnimation();

 private:
  friend class WorkspaceControllerTestHelper;

  aura::Window* viewport_;

  internal::ShelfLayoutManager* shelf_;
  scoped_ptr<internal::WorkspaceEventHandler> event_handler_;
  internal::WorkspaceLayoutManager* layout_manager_;

  DISALLOW_COPY_AND_ASSIGN(WorkspaceController);
};

}  
}  

#endif  
