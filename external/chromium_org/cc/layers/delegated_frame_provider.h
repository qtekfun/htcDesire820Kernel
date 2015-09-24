// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_DELEGATED_FRAME_PROVIDER_H_
#define CC_LAYERS_DELEGATED_FRAME_PROVIDER_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"
#include "cc/resources/return_callback.h"
#include "cc/resources/returned_resource.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/size.h"

namespace cc {
class DelegatedFrameData;
class DelegatedFrameResourceCollection;
class DelegatedRendererLayer;

class CC_EXPORT DelegatedFrameProvider
    : public base::RefCounted<DelegatedFrameProvider> {
 public:
  explicit DelegatedFrameProvider(
      const scoped_refptr<DelegatedFrameResourceCollection>&
          resource_collection,
      scoped_ptr<DelegatedFrameData> frame);

  void AddObserver(DelegatedRendererLayer* layer);
  void RemoveObserver(DelegatedRendererLayer* layer);

  void SetFrameData(scoped_ptr<DelegatedFrameData> frame);

  
  
  DelegatedFrameData* GetFrameDataAndRefResources(
      DelegatedRendererLayer* observer,
      gfx::RectF* damage);

  ReturnCallback GetReturnResourcesCallbackForImplThread();
  void UnrefResourcesOnMainThread(const ReturnedResourceArray& unused);

  gfx::Size frame_size() const { return frame_size_; }

 private:
  friend class base::RefCounted<DelegatedFrameProvider>;
  ~DelegatedFrameProvider();

  scoped_refptr<DelegatedFrameResourceCollection> resource_collection_;

  scoped_ptr<DelegatedFrameData> frame_;

  struct Observer {
    DelegatedRendererLayer* layer;
    gfx::RectF damage;

    Observer(DelegatedRendererLayer* layer, const gfx::RectF& damage)
        : layer(layer), damage(damage) {}
  };
  std::vector<Observer> observers_;

  gfx::Size frame_size_;

  DISALLOW_COPY_AND_ASSIGN(DelegatedFrameProvider);
};

}  

#endif  
