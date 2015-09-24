// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_BASE_SWAP_PROMISE_MONITOR_H_
#define CC_BASE_SWAP_PROMISE_MONITOR_H_

#include "cc/base/cc_export.h"

namespace cc {

class LayerTreeHost;
class LayerTreeHostImpl;

class CC_EXPORT SwapPromiseMonitor {
 public:
  
  
  
  
  SwapPromiseMonitor(LayerTreeHost* layer_tree_host,
                     LayerTreeHostImpl* layer_tree_host_impl);
  virtual ~SwapPromiseMonitor();

  virtual void OnSetNeedsCommitOnMain() = 0;
  virtual void OnSetNeedsRedrawOnImpl() = 0;

 protected:
  LayerTreeHost* layer_tree_host_;
  LayerTreeHostImpl* layer_tree_host_impl_;
};

}  

#endif  
