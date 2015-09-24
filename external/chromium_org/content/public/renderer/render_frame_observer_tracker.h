// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_RENDERER_RENDER_FRAME_OBSERVER_TRACKER_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_FRAME_OBSERVER_TRACKER_H_

#include <map>

#include "base/lazy_instance.h"

namespace content {

class RenderFrame;

template <class T>
class RenderFrameObserverTracker {
 public:
  static T* Get(const RenderFrame* render_frame) {
    return render_frame_map_.Get()[render_frame];
  }

  explicit RenderFrameObserverTracker(const RenderFrame* render_frame)
      : render_frame_(render_frame) {
    render_frame_map_.Get()[render_frame] = static_cast<T*>(this);
  }
  ~RenderFrameObserverTracker() {
    render_frame_map_.Get().erase(render_frame_);
  }

 private:
  const RenderFrame* render_frame_;

  static base::LazyInstance<std::map<const RenderFrame*, T*> >
    render_frame_map_;

  DISALLOW_COPY_AND_ASSIGN(RenderFrameObserverTracker<T>);
};

template <class T>
base::LazyInstance<std::map<const RenderFrame*, T*> >
    RenderFrameObserverTracker<T>::render_frame_map_ =
        LAZY_INSTANCE_INITIALIZER;

}  

#endif  
