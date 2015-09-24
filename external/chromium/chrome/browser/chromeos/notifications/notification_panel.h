// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_NOTIFICATION_PANEL_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_NOTIFICATION_PANEL_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/chromeos/frame/panel_controller.h"
#include "chrome/browser/chromeos/notifications/balloon_collection_impl.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/rect.h"

class Balloon;
class Notification;

namespace views {
class ScrollView;
}  

namespace chromeos {

class BalloonContainer;
class BalloonViewImpl;
class NotificationPanelTester;

class NotificationPanel : public PanelController::Delegate,
                          public BalloonCollectionImpl::NotificationUI,
                          public NotificationObserver {
 public:
  enum State {
    FULL,  
    KEEP_SIZE,  
    STICKY_AND_NEW,  
    MINIMIZED,  
    CLOSED,  
  };

  NotificationPanel();
  virtual ~NotificationPanel();

  
  void Show();
  void Hide();

  
  virtual void Add(Balloon* balloon);
  virtual bool Update(Balloon* balloon);
  virtual void Remove(Balloon* balloon);
  virtual void Show(Balloon* balloon);
  virtual void ResizeNotification(Balloon* balloon,
                                  const gfx::Size& size);
  virtual void SetActiveView(BalloonViewImpl* view);

  
  virtual string16 GetPanelTitle();
  virtual SkBitmap GetPanelIcon();
  virtual bool CanClosePanel();
  virtual void ClosePanel();
  virtual void ActivatePanel();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void OnMouseLeave();
  void OnMouseMotion(const gfx::Point& point);

  NotificationPanelTester* GetTester();

 private:
  friend class NotificationPanelTester;

  void Init();

  
  void UnregisterNotification();

  
  void UpdatePanel(bool update_panel_size);

  
  void ScrollBalloonToVisible(Balloon* balloon);

  
  void UpdateContainerBounds();

  
  void UpdateControl();

  
  
  
  gfx::Rect GetPreferredBounds();

  
  gfx::Rect GetStickyNewBounds();

  void StartStaleTimer(Balloon* balloon);

  
  
  void OnStale(BalloonViewImpl* view);

  
  void SetState(State, const char* method_name);

  
  void MarkStale(const Notification& notification);

  
  
  scoped_ptr<BalloonContainer> balloon_container_;

  
  views::Widget* panel_widget_;

  
  views::Widget* container_host_;

  
  
  
  scoped_ptr<PanelController> panel_controller_;

  
  scoped_ptr<views::ScrollView> scroll_view_;

  
  State state_;

  ScopedRunnableMethodFactory<NotificationPanel> task_factory_;

  
  gfx::Rect min_bounds_;

  
  int stale_timeout_;

  
  NotificationRegistrar registrar_;

  
  
  BalloonViewImpl* active_;

  
  Balloon* scroll_to_;

  
  scoped_ptr<NotificationPanelTester> tester_;

  DISALLOW_COPY_AND_ASSIGN(NotificationPanel);
};

class NotificationPanelTester {
 public:
  explicit NotificationPanelTester(NotificationPanel* panel)
      : panel_(panel) {
  }

  NotificationPanel::State state() {
    return panel_->state_;
  }

  
  int GetNotificationCount() const;

  
  int GetNewNotificationCount() const;

  
  int GetStickyNotificationCount() const;

  
  void SetStaleTimeout(int timeout);

  
  void MarkStale(const Notification& notification);

  
  PanelController* GetPanelController() const;

  
  BalloonViewImpl* GetBalloonView(BalloonCollectionImpl* collection,
                                  const Notification& notification);

  
  bool IsVisible(const BalloonViewImpl* view) const;

  
  bool IsActive(const BalloonViewImpl* view) const;

 private:
  NotificationPanel* panel_;
  DISALLOW_COPY_AND_ASSIGN(NotificationPanelTester);
};

}  

#endif  
