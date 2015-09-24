// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_OWNER_H_
#define UI_COMPOSITOR_LAYER_OWNER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/layer.h"

namespace ui {

class COMPOSITOR_EXPORT LayerOwner {
 public:
  LayerOwner();
  virtual ~LayerOwner();

  
  
  
  
  
  
  Layer* AcquireLayer() WARN_UNUSED_RESULT;

  ui::Layer* layer() { return layer_; }
  const ui::Layer* layer() const { return layer_; }

 protected:
  
  
  
  
  
  scoped_ptr<Layer> layer_owner_;
  Layer* layer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LayerOwner);
};

}  

#endif  
