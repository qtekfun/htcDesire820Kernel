// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_
#pragma once

#include <deque>

#include "base/basictypes.h"
#include "base/message_loop.h"
#include "chrome/browser/notifications/balloon_collection.h"
#include "chrome/browser/notifications/balloon_collection_base.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

#if defined(OS_MACOSX)
#define USE_OFFSETS 0
#else
#define USE_OFFSETS 1
#endif

class BalloonCollectionImpl : public BalloonCollection
#if USE_OFFSETS
                            , public MessageLoopForUI::Observer
#endif
{
 public:
  BalloonCollectionImpl();
  virtual ~BalloonCollectionImpl();

  
  virtual void Add(const Notification& notification,
                   Profile* profile);
  virtual bool RemoveById(const std::string& id);
  virtual bool RemoveBySourceOrigin(const GURL& source_origin);
  virtual void RemoveAll();
  virtual bool HasSpace() const;
  virtual void ResizeBalloon(Balloon* balloon, const gfx::Size& size);
  virtual void SetPositionPreference(PositionPreference position);
  virtual void DisplayChanged();
  virtual void OnBalloonClosed(Balloon* source);
  virtual const Balloons& GetActiveBalloons();

  
#if defined(OS_WIN)
  virtual void WillProcessMessage(const MSG& event) {}
  virtual void DidProcessMessage(const MSG& event);
#endif
#if defined(OS_LINUX)
  virtual void WillProcessEvent(GdkEvent* event) {}
  virtual void DidProcessEvent(GdkEvent* event);
#endif

 protected:
  
  
  class Layout {
   public:
    Layout();

    
    
    enum Placement {
      INVALID,
      VERTICALLY_FROM_TOP_LEFT,
      VERTICALLY_FROM_TOP_RIGHT,
      VERTICALLY_FROM_BOTTOM_LEFT,
      VERTICALLY_FROM_BOTTOM_RIGHT
    };

    
    void OnDisplaySettingsChanged();

    
    static int min_balloon_width() { return kBalloonMinWidth; }
    static int max_balloon_width() { return kBalloonMaxWidth; }
    static int min_balloon_height() { return kBalloonMinHeight; }
    static int max_balloon_height() { return kBalloonMaxHeight; }

    
    static gfx::Size ConstrainToSizeLimits(const gfx::Size& rect);

    void set_placement(Placement placement) { placement_ = placement; }

    
    
    
    
    
    void GetMaxLinearSize(int* max_balloon_size, int* total_size) const;

    
    
    
    
    bool RefreshSystemMetrics();

    
    
    gfx::Point GetLayoutOrigin() const;

    
    
    
    
    gfx::Point NextPosition(const gfx::Size& balloon_size,
                            gfx::Point* position_iterator) const;

    
    
    gfx::Point OffScreenLocation() const;

    
    
    bool RequiresOffsets() const;

   private:
    
    int VerticalEdgeMargin() const;
    int HorizontalEdgeMargin() const;
    int InterBalloonMargin() const;

    
    static const int kBalloonMinWidth = 300;
    static const int kBalloonMaxWidth = 300;
    static const int kBalloonMinHeight = 24;
    static const int kBalloonMaxHeight = 160;

    Placement placement_;
    gfx::Rect work_area_;
    DISALLOW_COPY_AND_ASSIGN(Layout);
  };

  
  
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile);

  
  gfx::Rect GetBalloonsBoundingBox() const;

 private:
  
  
  void PositionBalloons(bool is_reposition);

  
  void PositionBalloonsInternal(bool is_reposition);

#if defined(OS_MACOSX)
  
  static gfx::Rect GetMacWorkArea();
#endif

  
  BalloonCollectionBase base_;

  
  Layout layout_;

#if USE_OFFSETS
  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  
  void CancelOffsets();

  
  void HandleMouseMoveEvent();

  
  bool IsCursorInBalloonCollection() const;

  
  ScopedRunnableMethodFactory<BalloonCollectionImpl> reposition_factory_;

  
  bool added_as_message_loop_observer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BalloonCollectionImpl);
};

#endif  
