// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_VISIBILITY_CONTROLLER_H_
#define UI_VIEWS_COREWM_VISIBILITY_CONTROLLER_H_

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "ui/aura/client/visibility_client.h"
#include "ui/views/views_export.h"

namespace views {
namespace corewm {

class VIEWS_EXPORT VisibilityController
    : public aura::client::VisibilityClient {
 public:
  VisibilityController();
  virtual ~VisibilityController();

 protected:
  
  
  
  
  virtual bool CallAnimateOnChildWindowVisibilityChanged(aura::Window* window,
                                                         bool visible);

 private:
  
  virtual void UpdateLayerVisibility(aura::Window* window,
                                     bool visible) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(VisibilityController);
};

class VIEWS_EXPORT SuspendChildWindowVisibilityAnimations {
 public:
  
  explicit SuspendChildWindowVisibilityAnimations(aura::Window* window);

  
  ~SuspendChildWindowVisibilityAnimations();

 private:
  
  aura::Window* window_;

  
  const bool original_enabled_;

  DISALLOW_COPY_AND_ASSIGN(SuspendChildWindowVisibilityAnimations);
};

void VIEWS_EXPORT SetChildWindowVisibilityChangesAnimated(aura::Window* window);

}  
}  

#endif  
