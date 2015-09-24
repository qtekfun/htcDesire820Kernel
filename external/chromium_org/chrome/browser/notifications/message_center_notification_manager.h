// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_MESSAGE_CENTER_NOTIFICATION_MANAGER_H_
#define CHROME_BROWSER_NOTIFICATIONS_MESSAGE_CENTER_NOTIFICATION_MANAGER_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/notifications/message_center_stats_collector.h"
#include "chrome/browser/notifications/notification.h"
#include "chrome/browser/notifications/notification_system_observer.h"
#include "chrome/browser/notifications/notification_ui_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_observer.h"
#include "ui/message_center/message_center_tray_delegate.h"
#include "ui/message_center/message_center_types.h"

class MessageCenterSettingsController;
class Notification;
class PrefService;
class Profile;

namespace message_center {
class NotificationBlocker;
}

class MessageCenterNotificationManager
    : public NotificationUIManager,
      public message_center::MessageCenterObserver,
      public content::NotificationObserver {
 public:
  MessageCenterNotificationManager(
      message_center::MessageCenter* message_center,
      PrefService* local_state,
      scoped_ptr<message_center::NotifierSettingsProvider> settings_provider);
  virtual ~MessageCenterNotificationManager();

  
  virtual void Add(const Notification& notification,
                   Profile* profile) OVERRIDE;
  virtual bool Update(const Notification& notification,
                      Profile* profile) OVERRIDE;
  virtual const Notification* FindById(
      const std::string& notification_id) const OVERRIDE;
  virtual bool CancelById(const std::string& notification_id) OVERRIDE;
  virtual std::set<std::string> GetAllIdsByProfileAndSourceOrigin(
      Profile* profile,
      const GURL& source) OVERRIDE;
  virtual bool CancelAllBySourceOrigin(const GURL& source_origin) OVERRIDE;
  virtual bool CancelAllByProfile(Profile* profile) OVERRIDE;
  virtual void CancelAll() OVERRIDE;

  
  virtual void OnNotificationRemoved(const std::string& notification_id,
                                     bool by_user) OVERRIDE;
  virtual void OnCenterVisibilityChanged(message_center::Visibility) OVERRIDE;
  virtual void OnNotificationUpdated(const std::string& notification_id)
      OVERRIDE;

#if defined(OS_WIN)
  
  
  
  void DisplayFirstRunBalloon();

  void SetFirstRunTimeoutForTest(base::TimeDelta timeout);
  bool FirstRunTimerIsActive() const;
#endif

  
  void SetMessageCenterTrayDelegateForTest(
      message_center::MessageCenterTrayDelegate* delegate);

 protected:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  class ImageDownloadsObserver {
   public:
    virtual void OnDownloadsCompleted() = 0;
  };

  typedef base::Callback<void(const gfx::Image&)> SetImageCallback;
  class ImageDownloads
      : public base::SupportsWeakPtr<ImageDownloads> {
   public:
    ImageDownloads(
        message_center::MessageCenter* message_center,
        ImageDownloadsObserver* observer);
    virtual ~ImageDownloads();

    void StartDownloads(const Notification& notification);
    void StartDownloadWithImage(const Notification& notification,
                                const gfx::Image* image,
                                const GURL& url,
                                const SetImageCallback& callback);
    void StartDownloadByKey(const Notification& notification,
                            const char* key,
                            int size,
                            const SetImageCallback& callback);

    
    void DownloadComplete(const SetImageCallback& callback,
                          int download_id,
                          int http_status_code,
                          const GURL& image_url,
                          const std::vector<SkBitmap>& bitmaps,
                          const std::vector<gfx::Size>& original_bitmap_sizes);
   private:
    
    
    
    void AddPendingDownload();
    void PendingDownloadCompleted();

    
    message_center::MessageCenter* message_center_;

    
    size_t pending_downloads_;

    
    ImageDownloadsObserver* observer_;

    DISALLOW_COPY_AND_ASSIGN(ImageDownloads);
  };

  
  
  
  
  
  
  
  

  
  
  
  class ProfileNotification : public ImageDownloadsObserver {
   public:
    ProfileNotification(Profile* profile,
                        const Notification& notification,
                        message_center::MessageCenter* message_center);
    virtual ~ProfileNotification();

    void StartDownloads();

    
    virtual void OnDownloadsCompleted() OVERRIDE;

    Profile* profile() const { return profile_; }
    const Notification& notification() const { return notification_; }

    
    
    std::string GetExtensionId();

   private:
    
    Profile* profile_;
    Notification notification_;
    
    
    scoped_ptr<ImageDownloads> downloads_;
  };

  scoped_ptr<message_center::MessageCenterTrayDelegate> tray_;
  message_center::MessageCenter* message_center_;  

  
  typedef std::map<std::string, ProfileNotification*> NotificationMap;
  NotificationMap profile_notifications_;

  
  
  void AddProfileNotification(ProfileNotification* profile_notification);
  void RemoveProfileNotification(ProfileNotification* profile_notification);

  
  
  ProfileNotification* FindProfileNotification(const std::string& id) const;

#if defined(OS_WIN)
  
  
  void CheckFirstRunTimer();

  
  
  BooleanPrefMember first_run_pref_;

  
  
  
  base::OneShotTimer<MessageCenterNotificationManager> first_run_balloon_timer_;

  
  
  base::TimeDelta first_run_idle_timeout_;

  
  base::WeakPtrFactory<MessageCenterNotificationManager> weak_factory_;
#endif

  scoped_ptr<message_center::NotifierSettingsProvider> settings_provider_;

  
  ScopedVector<message_center::NotificationBlocker> blockers_;

  
  content::NotificationRegistrar registrar_;

  NotificationSystemObserver system_observer_;

  
  MessageCenterStatsCollector stats_collector_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterNotificationManager);
};

#endif  
