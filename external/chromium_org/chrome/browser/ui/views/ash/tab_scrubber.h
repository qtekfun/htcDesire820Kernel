// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ASH_TAB_SCRUBBER_H_
#define CHROME_BROWSER_UI_VIEWS_ASH_TAB_SCRUBBER_H_

#include "base/timer/timer.h"
#include "chrome/browser/ui/views/frame/immersive_mode_controller.h"
#include "chrome/browser/ui/views/tabs/tab_strip_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/events/event_handler.h"

class Browser;
class TabStrip;

namespace gfx {
class Point;
}

class TabScrubber : public ui::EventHandler,
                    public content::NotificationObserver,
                    public TabStripObserver {
 public:
  enum Direction {LEFT, RIGHT};

  
  static TabScrubber* GetInstance();

  
  
  static gfx::Point GetStartPoint(TabStrip* tab_strip,
                                  int index,
                                  TabScrubber::Direction direction);

  void set_activation_delay(int activation_delay) {
    activation_delay_ = activation_delay;
    use_default_activation_delay_ = false;
  }
  int activation_delay() const { return activation_delay_; }
  int highlighted_tab() const { return highlighted_tab_; }
  bool IsActivationPending();

 private:
  TabScrubber();
  virtual ~TabScrubber();

  
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabStripAddedTabAt(TabStrip* tab_strip, int index) OVERRIDE;
  virtual void TabStripMovedTab(TabStrip* tab_strip,
                                int from_index,
                                int to_index) OVERRIDE;
  virtual void TabStripRemovedTabAt(TabStrip* tab_strip, int index) OVERRIDE;
  virtual void TabStripDeleted(TabStrip* tab_strip) OVERRIDE;

  Browser* GetActiveBrowser();
  void FinishScrub(bool activate);

  
  bool scrubbing_;
  
  
  Browser* browser_;
  
  
  float swipe_x_;
  float swipe_y_;
  
  Direction swipe_direction_;
  
  int highlighted_tab_;
  
  base::Timer activate_timer_;
  
  int activation_delay_;
  
  bool use_default_activation_delay_;
  
  scoped_ptr<ImmersiveRevealedLock> immersive_reveal_lock_;

  content::NotificationRegistrar registrar_;
  base::WeakPtrFactory<TabScrubber> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TabScrubber);
};

#endif  
