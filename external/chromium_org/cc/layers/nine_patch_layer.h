// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_NINE_PATCH_LAYER_H_
#define CC_LAYERS_NINE_PATCH_LAYER_H_

#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"
#include "cc/layers/layer.h"
#include "cc/layers/ui_resource_layer.h"
#include "cc/resources/ui_resource_client.h"
#include "ui/gfx/rect.h"

namespace cc {

class LayerTreeHost;
class ScopedUIResource;

class CC_EXPORT NinePatchLayer : public UIResourceLayer {
 public:
  static scoped_refptr<NinePatchLayer> Create();

  virtual void PushPropertiesTo(LayerImpl* layer) OVERRIDE;

  
  
  
  
  
  void SetBorder(gfx::Rect border);

  
  
  
  
  
  
  void SetAperture(gfx::Rect aperture);
  void SetFillCenter(bool fill_center);

 private:
  NinePatchLayer();
  virtual ~NinePatchLayer();
  virtual scoped_ptr<LayerImpl> CreateLayerImpl(LayerTreeImpl* tree_impl)
      OVERRIDE;

  gfx::Rect border_;
  bool fill_center_;

  
  
  gfx::Rect image_aperture_;

  DISALLOW_COPY_AND_ASSIGN(NinePatchLayer);
};

}  

#endif  
