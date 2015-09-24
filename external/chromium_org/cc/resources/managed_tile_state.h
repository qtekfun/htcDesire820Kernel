// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_MANAGED_TILE_STATE_H_
#define CC_RESOURCES_MANAGED_TILE_STATE_H_

#include "base/memory/scoped_ptr.h"
#include "cc/resources/platform_color.h"
#include "cc/resources/raster_worker_pool.h"
#include "cc/resources/resource_pool.h"
#include "cc/resources/resource_provider.h"
#include "cc/resources/scoped_resource.h"

namespace cc {

class TileManager;

enum ManagedTileBin {
  NOW_AND_READY_TO_DRAW_BIN = 0,  
  NOW_BIN = 1,                    
  SOON_BIN = 2,                   
  EVENTUALLY_AND_ACTIVE_BIN = 3,  
  EVENTUALLY_BIN = 4,             
  AT_LAST_AND_ACTIVE_BIN = 5,     
  AT_LAST_BIN = 6,                
  NEVER_BIN = 7,                  
  NUM_BINS = 8
  
  
};
scoped_ptr<base::Value> ManagedTileBinAsValue(
    ManagedTileBin bin);

class CC_EXPORT ManagedTileState {
 public:
  class CC_EXPORT TileVersion {
    public:
      enum Mode {
        RESOURCE_MODE,
        SOLID_COLOR_MODE,
        PICTURE_PILE_MODE
      };

      TileVersion();
      ~TileVersion();

      Mode mode() const {
        return mode_;
      }

      bool IsReadyToDraw() const;

      ResourceProvider::ResourceId get_resource_id() const {
        DCHECK(mode_ == RESOURCE_MODE);
        DCHECK(resource_);

        return resource_->id();
      }

      SkColor get_solid_color() const {
        DCHECK(mode_ == SOLID_COLOR_MODE);

        return solid_color_;
      }

      bool contents_swizzled() const {
        DCHECK(resource_);
        return !PlatformColor::SameComponentOrder(resource_->format());
      }

      bool requires_resource() const {
        return mode_ == RESOURCE_MODE ||
               mode_ == PICTURE_PILE_MODE;
      }

      size_t GPUMemoryUsageInBytes() const;

      void SetSolidColorForTesting(SkColor color) {
        set_solid_color(color);
      }
      void SetHasTextForTesting(bool has_text) {
        has_text_ = has_text;
      }

    private:
      friend class TileManager;
      friend class PrioritizedTileSet;
      friend class Tile;
      friend class ManagedTileState;

      void set_use_resource() {
        mode_ = RESOURCE_MODE;
      }

      void set_solid_color(const SkColor& color) {
        mode_ = SOLID_COLOR_MODE;
        solid_color_ = color;
      }

      void set_has_text(bool has_text) {
        has_text_ = has_text;
      }

      void set_rasterize_on_demand() {
        mode_ = PICTURE_PILE_MODE;
      }

      Mode mode_;
      SkColor solid_color_;
      bool has_text_;
      scoped_ptr<ScopedResource> resource_;
      RasterWorkerPool::RasterTask raster_task_;
  };

  ManagedTileState();
  ~ManagedTileState();

  scoped_ptr<base::Value> AsValue() const;

  
  TileVersion tile_versions[NUM_RASTER_MODES];
  RasterMode raster_mode;

  ManagedTileBin bin;

  TileResolution resolution;
  bool required_for_activation;
  float time_to_needed_in_seconds;
  float distance_to_visible_in_pixels;
  bool visible_and_ready_to_draw;

  
  unsigned scheduled_priority;
};

}  

#endif  
