// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_ANIMATION_ANIMATION_REGISTRAR_H_
#define CC_ANIMATION_ANIMATION_REGISTRAR_H_

#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"

namespace cc {

class LayerAnimationController;

class CC_EXPORT AnimationRegistrar {
 public:
  typedef base::hash_map<int, LayerAnimationController*> AnimationControllerMap;

  static scoped_ptr<AnimationRegistrar> Create() {
    return make_scoped_ptr(new AnimationRegistrar());
  }

  virtual ~AnimationRegistrar();

  
  
  scoped_refptr<LayerAnimationController> GetAnimationControllerForId(int id);

  
  
  void DidActivateAnimationController(LayerAnimationController* controller);

  
  
  
  void DidDeactivateAnimationController(LayerAnimationController* controller);

  
  void RegisterAnimationController(LayerAnimationController* controller);

  
  void UnregisterAnimationController(LayerAnimationController* controller);

  const AnimationControllerMap& active_animation_controllers() const {
    return active_animation_controllers_;
  }

  const AnimationControllerMap& all_animation_controllers() const {
    return all_animation_controllers_;
  }

 private:
  AnimationRegistrar();

  AnimationControllerMap active_animation_controllers_;
  AnimationControllerMap all_animation_controllers_;

  DISALLOW_COPY_AND_ASSIGN(AnimationRegistrar);
};

}  

#endif  
