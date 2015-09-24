// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_COLLECTION_H_
#define UI_MESSAGE_CENTER_VIEWS_MESSAGE_POPUP_COLLECTION_H_

#include <list>
#include <map>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "ui/gfx/display.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/views/message_center_controller.h"
#include "ui/message_center/views/toast_contents_view.h"
#include "ui/views/widget/widget_observer.h"

namespace base {
class RunLoop;
}

namespace views {
class Widget;
}

namespace ash {
class WebNotificationTrayTest;
FORWARD_DECLARE_TEST(WebNotificationTrayTest, ManyPopupNotifications);
}

namespace message_center {
namespace test {
class MessagePopupCollectionTest;
}

class MessageCenter;
class MessageCenterTray;

enum PopupAlignment {
  POPUP_ALIGNMENT_TOP = 1 << 0,
  POPUP_ALIGNMENT_LEFT = 1 << 1,
  POPUP_ALIGNMENT_BOTTOM = 1 << 2,
  POPUP_ALIGNMENT_RIGHT = 1 << 3,
};

class MESSAGE_CENTER_EXPORT MessagePopupCollection
    : public MessageCenterController,
      public MessageCenterObserver,
      public gfx::DisplayObserver {
 public:
  
  
  
  
  MessagePopupCollection(gfx::NativeView parent,
                         MessageCenter* message_center,
                         MessageCenterTray* tray,
                         bool first_item_has_no_margin);
  virtual ~MessagePopupCollection();

  
  virtual void ClickOnNotification(const std::string& notification_id) OVERRIDE;
  virtual void RemoveNotification(const std::string& notification_id,
                                  bool by_user) OVERRIDE;
  virtual void DisableNotificationsFromThisSource(
      const NotifierId& notifier_id) OVERRIDE;
  virtual void ShowNotifierSettingsBubble() OVERRIDE;
  virtual bool HasClickedListener(const std::string& notification_id) OVERRIDE;
  virtual void ClickOnNotificationButton(const std::string& notification_id,
                                         int button_index) OVERRIDE;
  virtual void ExpandNotification(const std::string& notification_id) OVERRIDE;
  virtual void GroupBodyClicked(const std::string& last_notification_id)
      OVERRIDE;
  virtual void ExpandGroup(const NotifierId& notifier_id) OVERRIDE;
  virtual void RemoveGroup(const NotifierId& notifier_id) OVERRIDE;

  void MarkAllPopupsShown();

  
  
  void OnMouseEntered(ToastContentsView* toast_entered);
  void OnMouseExited(ToastContentsView* toast_exited);

  
  
  
  
  
  void IncrementDeferCounter();
  void DecrementDeferCounter();

  
  
  void DoUpdateIfPossible();

  
  
  void ForgetToast(ToastContentsView* toast);

  
  
  
  
  
  
  
  void SetDisplayInfo(const gfx::Rect& work_area,
                      const gfx::Rect& screen_bounds);

  
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

  
  gfx::NativeView parent() const { return parent_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(ash::WebNotificationTrayTest,
                           ManyPopupNotifications);
  friend class test::MessagePopupCollectionTest;
  friend class ash::WebNotificationTrayTest;
  typedef std::list<ToastContentsView*> Toasts;

  
  std::set<std::string> CloseAllWidgets();

  
  void RemoveToast(ToastContentsView* toast, bool mark_as_shown);

  
  int GetToastOriginX(const gfx::Rect& toast_bounds) const;

  
  
  void UpdateWidgets();

  
  void RepositionWidgets();

  
  
  
  void RepositionWidgetsWithTarget();

  void ComputePopupAlignment(gfx::Rect work_area, gfx::Rect screen_bounds);

  
  
  int GetBaseLine(ToastContentsView* last_toast) const;

  
  virtual void OnNotificationAdded(const std::string& notification_id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnNotificationUpdated(
      const std::string& notification_id) OVERRIDE;

  ToastContentsView* FindToast(const std::string& notification_id) const;

  
  
  
  
  void OnDeferTimerExpired();

  
  views::Widget* GetWidgetForTest(const std::string& id) const;
  void CreateRunLoopForTest();
  void WaitForTest();
  gfx::Rect GetToastRectAt(size_t index) const;

  gfx::NativeView parent_;
  MessageCenter* message_center_;
  MessageCenterTray* tray_;
  Toasts toasts_;
  gfx::Rect work_area_;
  int64 display_id_;

  
  
  PopupAlignment alignment_;

  int defer_counter_;

  
  
  ToastContentsView* latest_toast_entered_;

  
  
  
  
  bool user_is_closing_toasts_by_clicking_;
  scoped_ptr<base::OneShotTimer<MessagePopupCollection> > defer_timer_;
  
  
  
  int target_top_edge_;

  
  scoped_ptr<base::RunLoop> run_loop_for_test_;

  
  bool first_item_has_no_margin_;

  
  
  base::WeakPtrFactory<MessagePopupCollection> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessagePopupCollection);
};

}  

#endif 
