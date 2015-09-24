// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_IMAGE_GRID_H_
#define UI_VIEWS_COREWM_IMAGE_GRID_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/views/views_export.h"

namespace gfx {
class Image;
}  

namespace views {
namespace corewm {

class VIEWS_EXPORT ImageGrid {
 public:
  
  class VIEWS_EXPORT TestAPI {
   public:
    TestAPI(ImageGrid* grid) : grid_(grid) {}

    gfx::Rect top_left_clip_rect() const {
      return grid_->top_left_painter_->clip_rect_;
    }
    gfx::Rect top_right_clip_rect() const {
      return grid_->top_right_painter_->clip_rect_;
    }
    gfx::Rect bottom_left_clip_rect() const {
      return grid_->bottom_left_painter_->clip_rect_;
    }
    gfx::Rect bottom_right_clip_rect() const {
      return grid_->bottom_right_painter_->clip_rect_;
    }

    
    gfx::RectF GetTransformedLayerBounds(const ui::Layer& layer);

   private:
    ImageGrid* grid_;  

    DISALLOW_COPY_AND_ASSIGN(TestAPI);
  };

  ImageGrid();
  ~ImageGrid();

  ui::Layer* layer() { return layer_.get(); }
  int top_image_height() const { return top_image_height_; }
  int bottom_image_height() const { return bottom_image_height_; }
  int left_image_width() const { return left_image_width_; }
  int right_image_width() const { return right_image_width_; }

  
  ui::Layer* top_left_layer() const { return top_left_layer_.get(); }
  ui::Layer* top_layer() const { return top_layer_.get(); }
  ui::Layer* top_right_layer() const { return top_right_layer_.get(); }
  ui::Layer* left_layer() const { return left_layer_.get(); }
  ui::Layer* center_layer() const { return center_layer_.get(); }
  ui::Layer* right_layer() const { return right_layer_.get(); }
  ui::Layer* bottom_left_layer() const { return bottom_left_layer_.get(); }
  ui::Layer* bottom_layer() const { return bottom_layer_.get(); }
  ui::Layer* bottom_right_layer() const { return bottom_right_layer_.get(); }

  
  
  
  void SetImages(const gfx::Image* top_left_image,
                 const gfx::Image* top_image,
                 const gfx::Image* top_right_image,
                 const gfx::Image* left_image,
                 const gfx::Image* center_image,
                 const gfx::Image* right_image,
                 const gfx::Image* bottom_left_image,
                 const gfx::Image* bottom_image,
                 const gfx::Image* bottom_right_image);

  void SetSize(const gfx::Size& size);

  
  
  void SetContentBounds(const gfx::Rect& content_bounds_in_dip);

 private:
  
  class ImagePainter : public ui::LayerDelegate {
   public:
    ImagePainter(const gfx::Image* image) : image_(image) {}
    virtual ~ImagePainter() {}

    
    
    void SetClipRect(const gfx::Rect& clip_rect, ui::Layer* layer);

    
    virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
    virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
    virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

   private:
    friend class TestAPI;

    const gfx::Image* image_;  

    gfx::Rect clip_rect_;

    DISALLOW_COPY_AND_ASSIGN(ImagePainter);
  };

  
  static gfx::Size GetImageSize(const gfx::Image* image);

  
  static bool LayerExceedsSize(const ui::Layer* layer, const gfx::Size& size);

  
  
  
  void SetImage(const gfx::Image* image,
                scoped_ptr<ui::Layer>* layer_ptr,
                scoped_ptr<ImagePainter>* painter_ptr);

  
  
  
  void ScaleWidth(gfx::Size center,
                  ui::Layer* layer,
                  gfx::Transform& transform);
  void ScaleHeight(gfx::Size center,
                   ui::Layer* layer,
                   gfx::Transform& transform);

  
  scoped_ptr<ui::Layer> layer_;

  
  gfx::Size size_;

  
  
  int top_image_height_;
  int bottom_image_height_;
  int left_image_width_;
  int right_image_width_;

  
  
  
  int base_top_row_height_;
  int base_bottom_row_height_;
  int base_left_column_width_;
  int base_right_column_width_;

  
  
  scoped_ptr<ui::Layer> top_left_layer_;
  scoped_ptr<ui::Layer> top_layer_;
  scoped_ptr<ui::Layer> top_right_layer_;
  scoped_ptr<ui::Layer> left_layer_;
  scoped_ptr<ui::Layer> center_layer_;
  scoped_ptr<ui::Layer> right_layer_;
  scoped_ptr<ui::Layer> bottom_left_layer_;
  scoped_ptr<ui::Layer> bottom_layer_;
  scoped_ptr<ui::Layer> bottom_right_layer_;

  
  
  scoped_ptr<ImagePainter> top_left_painter_;
  scoped_ptr<ImagePainter> top_painter_;
  scoped_ptr<ImagePainter> top_right_painter_;
  scoped_ptr<ImagePainter> left_painter_;
  scoped_ptr<ImagePainter> center_painter_;
  scoped_ptr<ImagePainter> right_painter_;
  scoped_ptr<ImagePainter> bottom_left_painter_;
  scoped_ptr<ImagePainter> bottom_painter_;
  scoped_ptr<ImagePainter> bottom_right_painter_;

  DISALLOW_COPY_AND_ASSIGN(ImageGrid);
};

}  
}  

#endif  
