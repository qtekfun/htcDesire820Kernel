// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_STACKING_CONTROLLER_H_
#define ASH_WM_STACKING_CONTROLLER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/window_tree_client.h"

namespace ash {
namespace internal {
class AlwaysOnTopController;
}

class ASH_EXPORT StackingController : public aura::client::WindowTreeClient {
 public:
  StackingController();
  virtual ~StackingController();

  
  virtual aura::Window* GetDefaultParent(aura::Window* context,
                                         aura::Window* window,
                                         const gfx::Rect& bounds) OVERRIDE;

 private:
  
  
  
  
  
  aura::Window* GetSystemModalContainer(aura::Window* root,
                                        aura::Window* window) const;

  DISALLOW_COPY_AND_ASSIGN(StackingController);
};

}  

#endif  
