// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/notifications/balloon.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/path.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "views/view.h"

namespace views {
class Menu2;
class MenuButton;
class MouseEvent;
class TextButton;
class Widget;
}  

class Notification;
class NotificationDetails;
class NotificationSource;

namespace chromeos {

class BalloonViewHost;
class NotificationControlView;

class BalloonViewImpl : public BalloonView,
                        public views::View,
                        public NotificationObserver {
 public:
  BalloonViewImpl(bool sticky, bool controls, bool web_ui);
  virtual ~BalloonViewImpl();

  
  virtual void Layout();
  virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child);

  
  virtual void Show(Balloon* balloon);
  virtual void Update();
  virtual void Close(bool by_user);
  virtual void RepositionToBalloon();
  gfx::Size GetSize() const;
  virtual BalloonHost* GetHost() const;

  
  bool stale() const { return stale_; }

  
  void set_stale() { stale_ = true; }

  
  bool sticky() const { return sticky_; }

  
  bool closed() const { return closed_; }

  
  bool IsFor(const Notification& notification) const;

  
  void Activated();

  
  void Deactivated();

 private:
  friend class NotificationControlView;

  
  virtual gfx::Size GetPreferredSize() {
    return gfx::Size(1000, 1000);
  }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void CreateOptionsMenu();

  
  void DelayedClose(bool by_user);

  
  void DenyPermission();

  
  gfx::NativeView GetParentNativeView();

  
  Balloon* balloon_;

  
  BalloonViewHost* html_contents_;

  
  ScopedRunnableMethodFactory<BalloonViewImpl> method_factory_;

  
  scoped_ptr<views::Widget> control_view_host_;

  bool stale_;
  NotificationRegistrar notification_registrar_;
  
  bool sticky_;
  
  bool controls_;
  
  bool closed_;
  
  bool web_ui_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewImpl);
};

}  

#endif  
