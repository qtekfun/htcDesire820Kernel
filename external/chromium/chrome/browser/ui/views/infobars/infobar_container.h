// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_H_
#pragma once

#include <vector>

#include "base/compiler_specific.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkColor.h"

class InfoBar;
class InfoBarDelegate;
class TabContents;

class InfoBarContainer : public NotificationObserver {
 public:
  class Delegate {
   public:
    
    virtual SkColor GetInfoBarSeparatorColor() const = 0;

    
    
    virtual void InfoBarContainerStateChanged(bool is_animating) = 0;

    
    
    virtual bool DrawInfoBarArrows(int* x) const = 0;

   protected:
    virtual ~Delegate();
  };

  explicit InfoBarContainer(Delegate* delegate);
  virtual ~InfoBarContainer();

  
  
  
  void ChangeTabContents(TabContents* contents);

  
  
  
  int GetVerticalOverlap(int* total_height);

  
  
  
  
  
  
  
  
  
  void SetMaxTopArrowHeight(int height);

  
  
  
  void OnInfoBarStateChanged(bool is_animating);

  
  
  
  void RemoveDelegate(InfoBarDelegate* delegate);

  
  
  
  void RemoveInfoBar(InfoBar* infobar);

  const Delegate* delegate() const { return delegate_; }

 protected:
  
  
  
  void RemoveAllInfoBarsForDestruction();

  
  
  virtual void PlatformSpecificAddInfoBar(InfoBar* infobar) = 0;
  virtual void PlatformSpecificRemoveInfoBar(InfoBar* infobar) = 0;

 private:
  typedef std::vector<InfoBar*> InfoBars;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  
  
  void RemoveInfoBar(InfoBarDelegate* delegate, bool use_animation);

  
  
  
  
  
  enum CallbackStatus { NO_CALLBACK, WANT_CALLBACK };
  void AddInfoBar(InfoBar* infobar,
                  bool animate,
                  CallbackStatus callback_status);

  void UpdateInfoBarArrowTargetHeights();
  int ArrowTargetHeightForInfoBar(size_t infobar_index) const;

  NotificationRegistrar registrar_;
  Delegate* delegate_;
  TabContents* tab_contents_;
  InfoBars infobars_;

  
  int top_arrow_target_height_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarContainer);
};

#endif  
