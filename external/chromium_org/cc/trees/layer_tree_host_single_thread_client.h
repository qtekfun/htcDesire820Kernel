// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TREES_LAYER_TREE_HOST_SINGLE_THREAD_CLIENT_H_
#define CC_TREES_LAYER_TREE_HOST_SINGLE_THREAD_CLIENT_H_

namespace cc {

class LayerTreeHostSingleThreadClient {
 public:
  
  virtual void ScheduleComposite() = 0;
  
  
  virtual void ScheduleAnimation() = 0;

  
  
  
  
  
  virtual void DidPostSwapBuffers() = 0;
  virtual void DidCompleteSwapBuffers() = 0;
  virtual void DidAbortSwapBuffers() = 0;

 protected:
  virtual ~LayerTreeHostSingleThreadClient() {}
};

}  

#endif  
