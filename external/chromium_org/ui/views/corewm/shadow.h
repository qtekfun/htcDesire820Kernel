// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_SHADOW_H_
#define UI_VIEWS_COREWM_SHADOW_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer_animation_observer.h"
#include "ui/gfx/rect.h"
#include "ui/views/views_export.h"

namespace ui {
class Layer;
}  

namespace views {
namespace corewm {

class ImageGrid;

class VIEWS_EXPORT Shadow : public ui::ImplicitAnimationObserver {
 public:
  enum Style {
    
    
    STYLE_ACTIVE,

    
    STYLE_INACTIVE,

    
    
    STYLE_SMALL,
  };

  Shadow();
  virtual ~Shadow();

  void Init(Style style);

  
  
  
  
  ui::Layer* layer() const;

  const gfx::Rect& content_bounds() const { return content_bounds_; }
  Style style() const { return style_; }

  
  void SetContentBounds(const gfx::Rect& content_bounds);

  
  void SetStyle(Style style);

  
  virtual void OnImplicitAnimationsCompleted() OVERRIDE;

 private:
  
  void UpdateImagesForStyle();

  
  
  void UpdateImageGridBounds();

  
  Style style_;

  scoped_ptr<ImageGrid> image_grid_;

  
  gfx::Rect content_bounds_;

  
  
  int interior_inset_;

  DISALLOW_COPY_AND_ASSIGN(Shadow);
};

}  
}  

#endif  
