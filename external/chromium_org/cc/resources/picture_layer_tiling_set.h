// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_PICTURE_LAYER_TILING_SET_H_
#define CC_RESOURCES_PICTURE_LAYER_TILING_SET_H_

#include "cc/base/region.h"
#include "cc/base/scoped_ptr_vector.h"
#include "cc/resources/picture_layer_tiling.h"
#include "ui/gfx/size.h"

namespace cc {

class CC_EXPORT PictureLayerTilingSet {
 public:
  PictureLayerTilingSet(PictureLayerTilingClient* client,
                        gfx::Size layer_bounds);
  ~PictureLayerTilingSet();

  void SetClient(PictureLayerTilingClient* client);
  const PictureLayerTilingClient* client() const { return client_; }

  
  
  
  
  void SyncTilings(
     const PictureLayerTilingSet& other,
     gfx::Size new_layer_bounds,
     const Region& layer_invalidation,
     float minimum_contents_scale);

  gfx::Size layer_bounds() const { return layer_bounds_; }

  void SetCanUseLCDText(bool can_use_lcd_text);

  PictureLayerTiling* AddTiling(float contents_scale);
  size_t num_tilings() const { return tilings_.size(); }
  int NumHighResTilings() const;
  PictureLayerTiling* tiling_at(size_t idx) { return tilings_[idx]; }
  const PictureLayerTiling* tiling_at(size_t idx) const {
    return tilings_[idx];
  }

  PictureLayerTiling* TilingAtScale(float scale) const;

  
  void RemoveAllTilings();

  
  void Remove(PictureLayerTiling* tiling);

  
  void RemoveAllTiles();

  void UpdateTilePriorities(
      WhichTree tree,
      gfx::Size device_viewport,
      gfx::Rect viewport_in_content_space,
      gfx::Rect visible_content_rect,
      gfx::Size last_layer_bounds,
      gfx::Size current_layer_bounds,
      float last_layer_contents_scale,
      float current_layer_contents_scale,
      const gfx::Transform& last_screen_transform,
      const gfx::Transform& current_screen_transform,
      double current_frame_time_in_seconds,
      size_t max_tiles_for_interest_area);

  void DidBecomeActive();
  void DidBecomeRecycled();

  
  
  
  
  
  class CC_EXPORT CoverageIterator {
   public:
    CoverageIterator(const PictureLayerTilingSet* set,
      float contents_scale,
      gfx::Rect content_rect,
      float ideal_contents_scale);
    ~CoverageIterator();

    
    
    gfx::Rect geometry_rect() const;
    
    gfx::RectF texture_rect() const;
    
    gfx::Size texture_size() const;

    Tile* operator->() const;
    Tile* operator*() const;

    CoverageIterator& operator++();
    operator bool() const;

    PictureLayerTiling* CurrentTiling();

   private:
    int NextTiling() const;

    const PictureLayerTilingSet* set_;
    float contents_scale_;
    float ideal_contents_scale_;
    PictureLayerTiling::CoverageIterator tiling_iter_;
    int current_tiling_;
    int ideal_tiling_;

    Region current_region_;
    Region missing_region_;
    Region::Iterator region_iter_;
  };

  scoped_ptr<base::Value> AsValue() const;
  size_t GPUMemoryUsageInBytes() const;

 private:
  PictureLayerTilingClient* client_;
  gfx::Size layer_bounds_;
  ScopedPtrVector<PictureLayerTiling> tilings_;

  friend class Iterator;
  DISALLOW_COPY_AND_ASSIGN(PictureLayerTilingSet);
};

}  

#endif  
