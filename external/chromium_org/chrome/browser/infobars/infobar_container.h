// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INFOBAR_CONTAINER_H_
#define CHROME_BROWSER_INFOBARS_INFOBAR_CONTAINER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/skia/include/core/SkColor.h"

class InfoBar;
class InfoBarService;

class InfoBarContainer : public content::NotificationObserver {
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

  
  
  
  
  void ChangeInfoBarService(InfoBarService* infobar_service);

  
  
  
  int GetVerticalOverlap(int* total_height);

  
  
  
  
  
  
  
  
  
  void SetMaxTopArrowHeight(int height);

  
  
  
  void OnInfoBarStateChanged(bool is_animating);

  
  
  void RemoveInfoBar(InfoBar* infobar);

  const Delegate* delegate() const { return delegate_; }

 protected:
  
  
  
  void RemoveAllInfoBarsForDestruction();

  
  
  
  
  virtual void PlatformSpecificAddInfoBar(InfoBar* infobar,
                                          size_t position) = 0;
  
  
  
  virtual void PlatformSpecificReplaceInfoBar(InfoBar* old_infobar,
                                              InfoBar* new_infobar) {}
  virtual void PlatformSpecificRemoveInfoBar(InfoBar* infobar) = 0;
  virtual void PlatformSpecificInfoBarStateChanged(bool is_animating) {}

 private:
  typedef std::vector<InfoBar*> InfoBars;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void HideAllInfoBars();

  
  
  
  
  
  enum CallbackStatus { NO_CALLBACK, WANT_CALLBACK };
  void AddInfoBar(InfoBar* infobar,
                  size_t position,
                  bool animate,
                  CallbackStatus callback_status);

  void UpdateInfoBarArrowTargetHeights();
  int ArrowTargetHeightForInfoBar(size_t infobar_index) const;

  content::NotificationRegistrar registrar_;
  Delegate* delegate_;
  InfoBarService* infobar_service_;
  InfoBars infobars_;

  
  int top_arrow_target_height_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarContainer);
};

#endif  
