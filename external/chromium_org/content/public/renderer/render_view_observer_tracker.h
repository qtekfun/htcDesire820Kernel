// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_RENDERER_RENDER_VIEW_OBSERVER_TRACKER_H_
#define CONTENT_PUBLIC_RENDERER_RENDER_VIEW_OBSERVER_TRACKER_H_

#include <map>

#include "base/lazy_instance.h"

namespace content {

class RenderView;

template <class T>
class RenderViewObserverTracker {
 public:
  static T* Get(const RenderView* render_view) {
    return render_view_map_.Get()[render_view];
  }

  explicit RenderViewObserverTracker(const RenderView* render_view)
      : render_view_(render_view) {
    render_view_map_.Get()[render_view] = static_cast<T*>(this);
  }
  ~RenderViewObserverTracker() {
    render_view_map_.Get().erase(render_view_);
  }

 private:
  const RenderView* render_view_;

  static base::LazyInstance<std::map<const RenderView*, T*> >
    render_view_map_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewObserverTracker<T>);
};

template <class T>
base::LazyInstance<std::map<const RenderView*, T*> >
    RenderViewObserverTracker<T>::render_view_map_ = LAZY_INSTANCE_INITIALIZER;

}  

#endif  
