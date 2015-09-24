// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_COLLECTION_IMPL_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/notifications/balloon_view_host.h"
#include "chrome/browser/notifications/balloon_collection.h"
#include "chrome/browser/notifications/balloon_collection_base.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace gfx {
class Size;
}  

namespace chromeos {

class BalloonViewImpl;

class BalloonCollectionImpl : public BalloonCollection,
                              public NotificationObserver {
 public:
  
  
  class NotificationUI {
   public:
    NotificationUI() {}
    virtual ~NotificationUI() {}

    
    virtual void Add(Balloon* balloon) = 0;
    virtual bool Update(Balloon* balloon) = 0;
    virtual void Remove(Balloon* balloon) = 0;
    virtual void Show(Balloon* balloon) = 0;

    
    virtual void ResizeNotification(Balloon* balloon,
                                    const gfx::Size& size) = 0;

    
    virtual void SetActiveView(BalloonViewImpl* view) = 0;
   private:
    DISALLOW_COPY_AND_ASSIGN(NotificationUI);
  };

  BalloonCollectionImpl();
  virtual ~BalloonCollectionImpl();

  
  virtual void Add(const Notification& notification,
                   Profile* profile);
  virtual bool RemoveById(const std::string& id);
  virtual bool RemoveBySourceOrigin(const GURL& origin);
  virtual void RemoveAll();
  virtual bool HasSpace() const;
  virtual void ResizeBalloon(Balloon* balloon, const gfx::Size& size);
  virtual void SetPositionPreference(PositionPreference position) {}
  virtual void DisplayChanged() {}
  virtual void OnBalloonClosed(Balloon* source);
  virtual const Balloons& GetActiveBalloons() { return base_.balloons(); }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  
  bool AddWebUIMessageCallback(const Notification& notification,
                               const std::string& message,
                               MessageCallback* callback);

  
  
  
  
  void AddSystemNotification(const Notification& notification,
                             Profile* profile, bool sticky, bool controls);

  
  
  
  
  
  bool UpdateNotification(const Notification& notification);

  
  
  
  bool UpdateAndShowNotification(const Notification& notification);

  
  void set_notification_ui(NotificationUI* ui) {
    notification_ui_.reset(ui);
  }

  NotificationUI* notification_ui() {
    return notification_ui_.get();
  }

 protected:
  
  
  virtual Balloon* MakeBalloon(const Notification& notification,
                               Profile* profile);

  
  BalloonCollectionBase base_;

 private:
  friend class NotificationPanelTester;

  
  void Shutdown();

  Balloon* FindBalloon(const Notification& notification) {
    return base_.FindBalloon(notification);
  }

  scoped_ptr<NotificationUI> notification_ui_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BalloonCollectionImpl);
};

}  

#endif  
