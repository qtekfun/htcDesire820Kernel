// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_IMPL_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_IMPL_H_

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/stl_util.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/message_center_types.h"
#include "ui/message_center/notification_blocker.h"

namespace message_center {
class NotificationDelegate;
class MessageCenterImpl;

namespace internal {
class ChangeQueue;
class PopupTimersController;

class PopupTimer {
 public:
  
  
  
  PopupTimer(const std::string& id,
             base::TimeDelta timeout,
             base::WeakPtr<PopupTimersController> controller);
  ~PopupTimer();

  
  
  void Start();

  
  
  void Pause();

  
  
  void Reset();

  base::TimeDelta get_timeout() const { return timeout_; }

 private:
  
  const std::string id_;

  
  base::TimeDelta timeout_;

  
  base::TimeDelta passed_;

  
  base::Time start_time_;

  
  base::WeakPtr<PopupTimersController> timer_controller_;

  
  scoped_ptr<base::OneShotTimer<PopupTimersController> > timer_;

  DISALLOW_COPY_AND_ASSIGN(PopupTimer);
};

class MESSAGE_CENTER_EXPORT PopupTimersController
    : public base::SupportsWeakPtr<PopupTimersController>,
      public MessageCenterObserver {
 public:
  explicit PopupTimersController(MessageCenter* message_center);
  virtual ~PopupTimersController();

  
  virtual void OnNotificationDisplayed(const std::string& id) OVERRIDE;
  virtual void OnNotificationUpdated(const std::string& id) OVERRIDE;
  virtual void OnNotificationRemoved(const std::string& id, bool by_user)
      OVERRIDE;

  
  virtual void TimerFinished(const std::string& id);

  
  void PauseAll();

  
  void StartAll();

  
  void CancelAll();

  
  void StartTimer(const std::string& id,
                  const base::TimeDelta& timeout_in_seconds);

  
  void ResetTimer(const std::string& id,
                  const base::TimeDelta& timeout_in_seconds);

  
  
  void PauseTimer(const std::string& id);

  
  void CancelTimer(const std::string& id);

 private:
  
  MessageCenter* message_center_;

  
  
  
  typedef std::map<std::string, PopupTimer*> PopupTimerCollection;
  PopupTimerCollection popup_timers_;
  STLValueDeleter<PopupTimerCollection> popup_deleter_;

  DISALLOW_COPY_AND_ASSIGN(PopupTimersController);
};

}  

class MessageCenterImpl : public MessageCenter,
                          public NotificationBlocker::Observer {
 public:
  MessageCenterImpl();
  virtual ~MessageCenterImpl();

  
  virtual void AddObserver(MessageCenterObserver* observer) OVERRIDE;
  virtual void RemoveObserver(MessageCenterObserver* observer) OVERRIDE;
  virtual void AddNotificationBlocker(NotificationBlocker* blocker) OVERRIDE;
  virtual void RemoveNotificationBlocker(NotificationBlocker* blocker) OVERRIDE;
  virtual void SetVisibility(Visibility visible) OVERRIDE;
  virtual bool IsMessageCenterVisible() const OVERRIDE;
  virtual size_t NotificationCount() const OVERRIDE;
  virtual size_t UnreadNotificationCount() const OVERRIDE;
  virtual bool HasPopupNotifications() const OVERRIDE;
  virtual bool HasNotification(const std::string& id) OVERRIDE;
  virtual bool IsQuietMode() const OVERRIDE;
  virtual bool HasClickedListener(const std::string& id) OVERRIDE;
  virtual const NotificationList::Notifications& GetVisibleNotifications()
      OVERRIDE;
  virtual NotificationList::PopupNotifications GetPopupNotifications() OVERRIDE;
  virtual void AddNotification(scoped_ptr<Notification> notification) OVERRIDE;
  virtual void UpdateNotification(const std::string& old_id,
                                  scoped_ptr<Notification> new_notification)
      OVERRIDE;
  virtual void RemoveNotification(const std::string& id, bool by_user) OVERRIDE;
  virtual void RemoveAllNotifications(bool by_user) OVERRIDE;
  virtual void RemoveAllVisibleNotifications(bool by_user) OVERRIDE;
  virtual void SetNotificationIcon(const std::string& notification_id,
                                   const gfx::Image& image) OVERRIDE;
  virtual void SetNotificationImage(const std::string& notification_id,
                                    const gfx::Image& image) OVERRIDE;
  virtual void SetNotificationButtonIcon(const std::string& notification_id,
                                         int button_index,
                                         const gfx::Image& image) OVERRIDE;
  virtual void DisableNotificationsByNotifier(
      const NotifierId& notifier_id) OVERRIDE;
  virtual void ExpandNotification(const std::string& id) OVERRIDE;
  virtual void ClickOnNotification(const std::string& id) OVERRIDE;
  virtual void ClickOnNotificationButton(const std::string& id,
                                         int button_index) OVERRIDE;
  virtual void MarkSinglePopupAsShown(const std::string& id,
                                      bool mark_notification_as_read) OVERRIDE;
  virtual void DisplayedNotification(const std::string& id) OVERRIDE;
  virtual void SetNotifierSettingsProvider(
      NotifierSettingsProvider* provider) OVERRIDE;
  virtual NotifierSettingsProvider* GetNotifierSettingsProvider() OVERRIDE;
  virtual void SetQuietMode(bool in_quiet_mode) OVERRIDE;
  virtual void EnterQuietModeWithExpire(
      const base::TimeDelta& expires_in) OVERRIDE;
  virtual void RestartPopupTimers() OVERRIDE;
  virtual void PausePopupTimers() OVERRIDE;

  
  virtual void OnBlockingStateChanged(NotificationBlocker* blocker) OVERRIDE;

 protected:
  virtual void DisableTimersForTest() OVERRIDE;

 private:
  struct NotificationCache {
    NotificationCache();
    ~NotificationCache();
    void Rebuild(const NotificationList::Notifications& notificaitons);
    void RecountUnread();

    NotificationList::Notifications visible_notifications;
    size_t unread_count;
  };

  void RemoveNotifications(bool by_user, const NotificationBlockers& blockers);

  scoped_ptr<NotificationList> notification_list_;
  NotificationCache notification_cache_;
  ObserverList<MessageCenterObserver> observer_list_;
  scoped_ptr<internal::PopupTimersController> popup_timers_controller_;
  scoped_ptr<base::OneShotTimer<MessageCenterImpl> > quiet_mode_timer_;
  NotifierSettingsProvider* settings_provider_;
  std::vector<NotificationBlocker*> blockers_;

  
  
  scoped_ptr<internal::ChangeQueue> notification_queue_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterImpl);
};

}  

#endif  
