// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_
#define CHROME_BROWSER_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_

#include <deque>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/notifications/balloon_collection.h"
#include "chrome/browser/notifications/balloon_collection_base.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

#if defined(OS_MACOSX)
#define USE_OFFSETS 0
#else
#define USE_OFFSETS 1
#endif

class BalloonCollectionImpl : public BalloonCollection,
                              public content::NotificationObserver
#if USE_OFFSETS
                            , public base::MessageLoopForUI::Observer
#endif
{
 public:
  BalloonCollectionImpl();
  virtual ~BalloonCollectionImpl();

  
  virtual void Add(const Notification& notification,
                   Profile* profile) OVERRIDE;
  virtual const Notification* FindById(const std::string& id) const OVERRIDE;
  virtual bool RemoveById(const std::string& id) OVERRIDE;
  virtual bool RemoveBySourceOrigin(const GURL& source_origin) OVERRIDE;
  virtual bool RemoveByProfile(Profile* profile) OVERRIDE;
  virtual void RemoveAll() OVERRIDE;
  virtual bool HasSpace() const OVERRIDE;
  virtual void ResizeBalloon(Balloon* balloon, const gfx::Size& size) OVERRIDE;
  virtual void SetPositionPreference(PositionPreference position) OVERRIDE;
  virtual void DisplayChanged() OVERRIDE;
  virtual void OnBalloonClosed(Balloon* source) OVERRIDE;
  virtual const Balloons& GetActiveBalloons() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
#if defined(OS_WIN) || defined(USE_AURA)
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;
#elif defined(TOOLKIT_GTK)
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;
#endif

  
  
  int count() const { return base_.count(); }

  static int min_balloon_width() { return Layout::min_balloon_width(); }
  static int max_balloon_width() { return Layout::max_balloon_width(); }
  static int min_balloon_height() { return Layout::min_balloon_height(); }
  static int max_balloon_height() { return Layout::max_balloon_height(); }

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

    
    
    bool ComputeOffsetToMoveAbovePanels();

    void enable_computing_panel_offset() {
      need_to_compute_panel_offset_ = true;
    }

   private:
    
    int VerticalEdgeMargin() const;
    int HorizontalEdgeMargin() const;
    int InterBalloonMargin() const;

    bool NeedToMoveAboveLeftSidePanels() const;
    bool NeedToMoveAboveRightSidePanels() const;

    
    static const int kBalloonMinWidth = 300;
    static const int kBalloonMaxWidth = 300;
    static const int kBalloonMinHeight = 24;
    static const int kBalloonMaxHeight = 160;

    Placement placement_;
    gfx::Rect work_area_;

    
    
    bool need_to_compute_panel_offset_;

    
    
    
    int offset_to_move_above_panels_;

    DISALLOW_COPY_AND_ASSIGN(Layout);
  };

  
  
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile);

  
  
  void AddImpl(const Notification& notification,
               Profile* profile,
               bool add_to_front);

  
  gfx::Rect GetBalloonsBoundingBox() const;

  BalloonCollectionBase& base() { return base_; }
  Layout& layout() { return layout_; }

 private:
  
  
  void PositionBalloons(bool is_reposition);

  
  void PositionBalloonsInternal(bool is_reposition);

#if defined(OS_MACOSX)
  
  static gfx::Rect GetMacWorkArea();
#endif

  
  BalloonCollectionBase base_;

  
  Layout layout_;

  content::NotificationRegistrar registrar_;

#if USE_OFFSETS
  
  void AddMessageLoopObserver();
  void RemoveMessageLoopObserver();

  
  void CancelOffsets();

  
  void HandleMouseMoveEvent();

  
  bool IsCursorInBalloonCollection() const;

  
  base::WeakPtrFactory<BalloonCollectionImpl> reposition_factory_;

  
  bool added_as_message_loop_observer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BalloonCollectionImpl);
};

#endif  
