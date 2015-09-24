// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_PICTURE_PILE_IMPL_H_
#define CC_RESOURCES_PICTURE_PILE_IMPL_H_

#include <list>
#include <map>
#include <set>
#include <vector>

#include "base/time/time.h"
#include "cc/base/cc_export.h"
#include "cc/debug/rendering_stats_instrumentation.h"
#include "cc/resources/picture_pile_base.h"
#include "skia/ext/analysis_canvas.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkPicture.h"

namespace cc {

class CC_EXPORT PicturePileImpl : public PicturePileBase {
 public:
  static scoped_refptr<PicturePileImpl> Create();
  static scoped_refptr<PicturePileImpl> CreateFromOther(
      const PicturePileBase* other);

  
  PicturePileImpl* GetCloneForDrawingOnThread(unsigned thread_index) const;

  
  
  
  
  
  
  
  
  void RasterDirect(
      SkCanvas* canvas,
      gfx::Rect canvas_rect,
      float contents_scale,
      RenderingStatsInstrumentation* rendering_stats_instrumentation);

  
  
  
  void RasterToBitmap(
      SkCanvas* canvas,
      gfx::Rect canvas_rect,
      float contents_scale,
      RenderingStatsInstrumentation* stats_instrumentation);

  
  
  void RasterForAnalysis(
      skia::AnalysisCanvas* canvas,
      gfx::Rect canvas_rect,
      float contents_scale,
      RenderingStatsInstrumentation* stats_instrumentation);

  skia::RefPtr<SkPicture> GetFlattenedPicture();

  struct CC_EXPORT Analysis {
    Analysis();
    ~Analysis();

    bool is_solid_color;
    bool has_text;
    SkColor solid_color;
  };

  void AnalyzeInRect(gfx::Rect content_rect,
                     float contents_scale,
                     Analysis* analysis);

  void AnalyzeInRect(gfx::Rect content_rect,
                     float contents_scale,
                     Analysis* analysis,
                     RenderingStatsInstrumentation* stats_instrumentation);

  class CC_EXPORT PixelRefIterator {
   public:
    PixelRefIterator(gfx::Rect content_rect,
                     float contents_scale,
                     const PicturePileImpl* picture_pile);
    ~PixelRefIterator();

    skia::LazyPixelRef* operator->() const { return *pixel_ref_iterator_; }
    skia::LazyPixelRef* operator*() const { return *pixel_ref_iterator_; }
    PixelRefIterator& operator++();
    operator bool() const { return pixel_ref_iterator_; }

   private:
    void AdvanceToTilePictureWithPixelRefs();

    const PicturePileImpl* picture_pile_;
    gfx::Rect layer_rect_;
    TilingData::Iterator tile_iterator_;
    Picture::PixelRefIterator pixel_ref_iterator_;
    std::set<const void*> processed_pictures_;
  };

  void DidBeginTracing();

 protected:
  friend class PicturePile;
  friend class PixelRefIterator;

  PicturePileImpl();
  explicit PicturePileImpl(const PicturePileBase* other);
  virtual ~PicturePileImpl();

 private:
  class ClonesForDrawing {
   public:
    ClonesForDrawing(const PicturePileImpl* pile, int num_threads);
    ~ClonesForDrawing();

    typedef std::vector<scoped_refptr<PicturePileImpl> > PicturePileVector;
    PicturePileVector clones_;
  };

  static scoped_refptr<PicturePileImpl> CreateCloneForDrawing(
      const PicturePileImpl* other, unsigned thread_index);

  PicturePileImpl(const PicturePileImpl* other, unsigned thread_index);

 private:
  typedef std::map<Picture*, Region> PictureRegionMap;
  void CoalesceRasters(gfx::Rect canvas_rect,
                       gfx::Rect content_rect,
                       float contents_scale,
                       PictureRegionMap* result);

  void RasterCommon(
      SkCanvas* canvas,
      SkDrawPictureCallback* callback,
      gfx::Rect canvas_rect,
      float contents_scale,
      RenderingStatsInstrumentation* rendering_stats_instrumentation,
      bool is_analysis);

  
  
  
  
  const ClonesForDrawing clones_for_drawing_;

  DISALLOW_COPY_AND_ASSIGN(PicturePileImpl);
};

}  

#endif  
