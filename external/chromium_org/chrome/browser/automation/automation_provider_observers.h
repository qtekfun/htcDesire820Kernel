// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_OBSERVERS_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_OBSERVERS_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/synchronization/waitable_event.h"
#include "base/values.h"
#include "chrome/browser/automation/automation_provider_json.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "components/autofill/core/browser/personal_data_manager.h"
#include "components/autofill/core/browser/personal_data_manager_observer.h"
#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/login/enrollment/enrollment_screen.h"
#include "chrome/browser/chromeos/login/login_status_consumer.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/chromeos/login/wizard_controller.h"
#endif  
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "chrome/browser/memory_details.h"
#include "chrome/browser/password_manager/password_store_change.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/automation_constants.h"
#include "chrome/common/extensions/extension_constants.h"
#include "chrome/common/importer/importer_data_types.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/download_item.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

class AutomationProvider;
class BalloonCollection;
class Browser;
class ExtensionService;
class Notification;
class Profile;
class SavePackage;

namespace automation {
class Error;
}

#if defined(OS_CHROMEOS)
namespace chromeos {
class ExistingUserController;
class WizardScreen;
}
#endif  

namespace content {
class NavigationController;
class RenderViewHost;
class WebContents;
}

namespace extensions {
class Extension;
class ProcessManager;
}

namespace history {
class TopSites;
}

namespace IPC {
class Message;
}

namespace policy {
class BrowserPolicyConnector;
}

class InitialLoadObserver : public content::NotificationObserver {
 public:
  InitialLoadObserver(size_t tab_count, AutomationProvider* automation);
  virtual ~InitialLoadObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  
  
  
  base::DictionaryValue* GetTimingInformation() const;

 private:
  class TabTime;
  typedef std::map<uintptr_t, TabTime> TabTimeMap;
  typedef std::set<uintptr_t> TabSet;

  void ConditionMet();

  content::NotificationRegistrar registrar_;

  base::WeakPtr<AutomationProvider> automation_;
  size_t crashed_tab_count_;
  size_t outstanding_tab_count_;
  base::TimeTicks init_time_;
  TabTimeMap loading_tabs_;
  TabSet finished_tabs_;

  DISALLOW_COPY_AND_ASSIGN(InitialLoadObserver);
};

#if defined(OS_CHROMEOS)
class OOBEWebuiReadyObserver : public content::NotificationObserver {
 public:
  explicit OOBEWebuiReadyObserver(AutomationProvider* automation);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void OOBEWebuiReady();

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;

  DISALLOW_COPY_AND_ASSIGN(OOBEWebuiReadyObserver);
};
#endif  

class NewTabUILoadObserver : public content::NotificationObserver {
 public:
  explicit NewTabUILoadObserver(AutomationProvider* automation,
                                Profile* profile);
  virtual ~NewTabUILoadObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;

  DISALLOW_COPY_AND_ASSIGN(NewTabUILoadObserver);
};

class NavigationControllerRestoredObserver
    : public content::NotificationObserver {
 public:
  NavigationControllerRestoredObserver(
      AutomationProvider* automation,
      content::NavigationController* controller,
      IPC::Message* reply_message);
  virtual ~NavigationControllerRestoredObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  bool FinishedRestoring();
  void SendDone();

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  content::NavigationController* controller_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NavigationControllerRestoredObserver);
};

class NavigationNotificationObserver : public content::NotificationObserver {
 public:
  NavigationNotificationObserver(content::NavigationController* controller,
                                 AutomationProvider* automation,
                                 IPC::Message* reply_message,
                                 int number_of_navigations,
                                 bool include_current_navigation,
                                 bool use_json_interface);
  virtual ~NavigationNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void ConditionMet(AutomationMsg_NavigationResponseValues navigation_result);

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  content::NavigationController* controller_;
  int navigations_remaining_;
  bool navigation_started_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(NavigationNotificationObserver);
};

class TabStripNotificationObserver : public content::NotificationObserver {
 public:
  TabStripNotificationObserver(int notification,
                               AutomationProvider* automation);
  virtual ~TabStripNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  virtual void ObserveTab(content::NavigationController* controller) = 0;

 protected:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  int notification_;
};

class TabAppendedNotificationObserver : public TabStripNotificationObserver {
 public:
  TabAppendedNotificationObserver(Browser* parent,
                                  AutomationProvider* automation,
                                  IPC::Message* reply_message,
                                  bool use_json_interface);
  virtual ~TabAppendedNotificationObserver();

  virtual void ObserveTab(content::NavigationController* controller);
  IPC::Message* ReleaseReply();

 protected:
  Browser* parent_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TabAppendedNotificationObserver);
};

class TabClosedNotificationObserver : public TabStripNotificationObserver {
 public:
  TabClosedNotificationObserver(AutomationProvider* automation,
                                bool wait_until_closed,
                                IPC::Message* reply_message,
                                bool use_json_interface);
  virtual ~TabClosedNotificationObserver();

  virtual void ObserveTab(content::NavigationController* controller);

  void set_for_browser_command(bool for_browser_command);

 protected:
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;
  bool for_browser_command_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TabClosedNotificationObserver);
};

class TabCountChangeObserver : public TabStripModelObserver {
 public:
  TabCountChangeObserver(AutomationProvider* automation,
                         Browser* browser,
                         IPC::Message* reply_message,
                         int target_tab_count);
  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabStripModelDeleted() OVERRIDE;

 private:
  virtual ~TabCountChangeObserver();

  
  
  void CheckTabCount();

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  TabStripModel* tab_strip_model_;

  const int target_tab_count_;

  DISALLOW_COPY_AND_ASSIGN(TabCountChangeObserver);
};

class ExtensionUninstallObserver : public content::NotificationObserver {
 public:
  ExtensionUninstallObserver(AutomationProvider* automation,
                             IPC::Message* reply_message,
                             const std::string& id);
  virtual ~ExtensionUninstallObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  std::string id_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUninstallObserver);
};

class ExtensionReadyNotificationObserver
    : public content::NotificationObserver {
 public:
  
  ExtensionReadyNotificationObserver(extensions::ProcessManager* manager,
                                     ExtensionService* service,
                                     AutomationProvider* automation,
                                     IPC::Message* reply_message);
  virtual ~ExtensionReadyNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void Init();

  content::NotificationRegistrar registrar_;
  extensions::ProcessManager* manager_;
  ExtensionService* service_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  const extensions::Extension* extension_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionReadyNotificationObserver);
};

class ExtensionUnloadNotificationObserver
    : public content::NotificationObserver {
 public:
  ExtensionUnloadNotificationObserver();
  virtual ~ExtensionUnloadNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  bool did_receive_unload_notification() {
    return did_receive_unload_notification_;
  }

 private:
  content::NotificationRegistrar registrar_;
  bool did_receive_unload_notification_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUnloadNotificationObserver);
};

class ExtensionsUpdatedObserver : public content::NotificationObserver {
 public:
  ExtensionsUpdatedObserver(extensions::ProcessManager* manager,
                            AutomationProvider* automation,
                            IPC::Message* reply_message);
  virtual ~ExtensionsUpdatedObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void UpdateCheckFinished();

 private:
  void MaybeReply();

  content::NotificationRegistrar registrar_;
  extensions::ProcessManager* manager_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool updater_finished_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionsUpdatedObserver);
};

class BrowserOpenedNotificationObserver : public content::NotificationObserver {
 public:
  BrowserOpenedNotificationObserver(AutomationProvider* automation,
                                    IPC::Message* reply_message,
                                    bool use_json_interface);
  virtual ~BrowserOpenedNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void set_for_browser_command(bool for_browser_command);

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int new_window_id_;
  bool use_json_interface_;
  bool for_browser_command_;

  DISALLOW_COPY_AND_ASSIGN(BrowserOpenedNotificationObserver);
};

class BrowserClosedNotificationObserver : public content::NotificationObserver {
 public:
  BrowserClosedNotificationObserver(Browser* browser,
                                    AutomationProvider* automation,
                                    IPC::Message* reply_message,
                                    bool use_json_interface);
  virtual ~BrowserClosedNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void set_for_browser_command(bool for_browser_command);

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;
  bool for_browser_command_;

  DISALLOW_COPY_AND_ASSIGN(BrowserClosedNotificationObserver);
};

class BrowserCountChangeNotificationObserver
    : public content::NotificationObserver {
 public:
  BrowserCountChangeNotificationObserver(int target_count,
                                         AutomationProvider* automation,
                                         IPC::Message* reply_message);
  virtual ~BrowserCountChangeNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  int target_count_;
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(BrowserCountChangeNotificationObserver);
};

class ExecuteBrowserCommandObserver : public content::NotificationObserver {
 public:
  virtual ~ExecuteBrowserCommandObserver();

  static bool CreateAndRegisterObserver(AutomationProvider* automation,
                                        Browser* browser,
                                        int command,
                                        IPC::Message* reply_message,
                                        bool use_json_interface);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  ExecuteBrowserCommandObserver(AutomationProvider* automation,
                                IPC::Message* reply_message,
                                bool use_json_interface);

  bool Register(int command);

  bool Getint(int command, int* type);

  IPC::Message* ReleaseReply();

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  int notification_type_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(ExecuteBrowserCommandObserver);
};

class FindInPageNotificationObserver : public content::NotificationObserver {
 public:
  FindInPageNotificationObserver(AutomationProvider* automation,
                                 content::WebContents* parent_tab,
                                 bool reply_with_json,
                                 IPC::Message* reply_message);
  virtual ~FindInPageNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  
  
  
  static const int kFindInPageRequestId;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  
  
  int active_match_ordinal_;
  
  bool reply_with_json_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(FindInPageNotificationObserver);
};

class DomOperationObserver : public content::NotificationObserver {
 public:
  explicit DomOperationObserver(int automation_id);
  virtual ~DomOperationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  virtual void OnDomOperationCompleted(const std::string& json) = 0;
  virtual void OnJavascriptBlocked() = 0;

 private:
  int automation_id_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(DomOperationObserver);
};

class DomOperationMessageSender : public DomOperationObserver {
 public:
  DomOperationMessageSender(AutomationProvider* automation,
                            IPC::Message* reply_message,
                            bool use_json_interface);
  virtual ~DomOperationMessageSender();

  virtual void OnDomOperationCompleted(const std::string& json) OVERRIDE;
  virtual void OnJavascriptBlocked() OVERRIDE;

 private:
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(DomOperationMessageSender);
};

class MetricEventDurationObserver : public content::NotificationObserver {
 public:
  MetricEventDurationObserver();
  virtual ~MetricEventDurationObserver();

  
  int GetEventDurationMs(const std::string& event_name);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;

  typedef std::map<std::string, int> EventDurationMap;
  EventDurationMap durations_;

  DISALLOW_COPY_AND_ASSIGN(MetricEventDurationObserver);
};

class InfoBarCountObserver : public content::NotificationObserver {
 public:
  InfoBarCountObserver(AutomationProvider* automation,
                       IPC::Message* reply_message,
                       content::WebContents* web_contents,
                       size_t target_count);
  virtual ~InfoBarCountObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  
  void CheckCount();

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  content::WebContents* web_contents_;

  const size_t target_count_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarCountObserver);
};

#if defined(OS_CHROMEOS)
class LoginObserver : public chromeos::LoginStatusConsumer {
 public:
  LoginObserver(chromeos::ExistingUserController* controller,
                AutomationProvider* automation,
                IPC::Message* reply_message);

  virtual ~LoginObserver();

  virtual void OnLoginFailure(const chromeos::LoginFailure& error);

  virtual void OnLoginSuccess(const chromeos::UserContext& user_context);

 private:
  chromeos::ExistingUserController* controller_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(LoginObserver);
};

class WizardControllerObserver : public chromeos::WizardController::Observer,
                                 public content::NotificationObserver {
 public:
  WizardControllerObserver(chromeos::WizardController* wizard_controller,
                           AutomationProvider* automation,
                           IPC::Message* reply_message);
  virtual ~WizardControllerObserver();

  
  std::string screen_to_wait_for() { return screen_to_wait_for_; }
  void set_screen_to_wait_for(const std::string& screen_name) {
    screen_to_wait_for_ = screen_name;
  }

 protected:
  
  virtual void OnScreenChanged(chromeos::WizardScreen* next_screen) OVERRIDE;
  virtual void OnSessionStart() OVERRIDE;

  
  void Observe(int type,
               const content::NotificationSource& source,
               const content::NotificationDetails& details);

  
  void SendReply(const std::string& screen_name);

  content::NotificationRegistrar registrar_;
  chromeos::WizardController* wizard_controller_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  std::string screen_to_wait_for_;

  DISALLOW_COPY_AND_ASSIGN(WizardControllerObserver);
};

class ScreenLockUnlockObserver : public content::NotificationObserver {
 public:
  
  
  ScreenLockUnlockObserver(AutomationProvider* automation,
                           IPC::Message* reply_message,
                           bool lock_screen);
  virtual ~ScreenLockUnlockObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 protected:
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

 private:
  content::NotificationRegistrar registrar_;
  bool lock_screen_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLockUnlockObserver);
};

class ScreenUnlockObserver : public ScreenLockUnlockObserver,
                             public chromeos::LoginStatusConsumer {
 public:
  ScreenUnlockObserver(AutomationProvider* automation,
                       IPC::Message* reply_message);
  virtual ~ScreenUnlockObserver();

  virtual void OnLoginFailure(const chromeos::LoginFailure& error);

  virtual void OnLoginSuccess(const chromeos::UserContext& user_context) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ScreenUnlockObserver);
};

#endif  

class AutomationProviderBookmarkModelObserver : public BookmarkModelObserver {
 public:
  AutomationProviderBookmarkModelObserver(AutomationProvider* provider,
                                          IPC::Message* reply_message,
                                          BookmarkModel* model,
                                          bool use_json_interface);
  virtual ~AutomationProviderBookmarkModelObserver();

  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE {}
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE {}
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE {}
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE {}
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE {}
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE {}
  virtual void BookmarkNodeChildrenReordered(
      BookmarkModel* model,
      const BookmarkNode* node) OVERRIDE {}

  IPC::Message* ReleaseReply();

 private:
  
  
  
  void ReplyAndDelete(bool success);

  base::WeakPtr<AutomationProvider> automation_provider_;
  scoped_ptr<IPC::Message> reply_message_;
  BookmarkModel* model_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderBookmarkModelObserver);
};

class AutomationProviderDownloadUpdatedObserver
    : public content::DownloadItem::Observer {
 public:
  AutomationProviderDownloadUpdatedObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      bool wait_for_open,
      bool incognito);
  virtual ~AutomationProviderDownloadUpdatedObserver();

  virtual void OnDownloadUpdated(content::DownloadItem* download);
  virtual void OnDownloadOpened(content::DownloadItem* download);

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  bool wait_for_open_;
  bool incognito_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderDownloadUpdatedObserver);
};

class AutomationProviderDownloadModelChangedObserver
    : public AllDownloadItemNotifier::Observer {
 public:
  AutomationProviderDownloadModelChangedObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      content::DownloadManager* download_manager);
  virtual ~AutomationProviderDownloadModelChangedObserver();

  virtual void OnDownloadCreated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void OnDownloadRemoved(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;

 private:
  void ModelChanged();

  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  AllDownloadItemNotifier notifier_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderDownloadModelChangedObserver);
};

class AllDownloadsCompleteObserver
    : public content::DownloadManager::Observer,
      public content::DownloadItem::Observer {
 public:
  AllDownloadsCompleteObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      content::DownloadManager* download_manager,
      ListValue* pre_download_ids);
  virtual ~AllDownloadsCompleteObserver();

  
  virtual void OnDownloadCreated(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void ManagerGoingDown(content::DownloadManager* manager) OVERRIDE;

  
  virtual void OnDownloadUpdated(content::DownloadItem* download) OVERRIDE;

 private:
  void ReplyIfNecessary();

  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  content::DownloadManager* download_manager_;
  std::set<int> pre_download_ids_;
  std::set<content::DownloadItem*> pending_downloads_;

  DISALLOW_COPY_AND_ASSIGN(AllDownloadsCompleteObserver);
};

class AutomationProviderSearchEngineObserver
    : public TemplateURLServiceObserver {
 public:
  AutomationProviderSearchEngineObserver(
      AutomationProvider* provider,
      Profile* profile,
      IPC::Message* reply_message);
  virtual ~AutomationProviderSearchEngineObserver();

  virtual void OnTemplateURLServiceChanged();

 private:
  base::WeakPtr<AutomationProvider> provider_;
  Profile* profile_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderSearchEngineObserver);
};

class AutomationProviderHistoryObserver {
 public:
  AutomationProviderHistoryObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message);
  virtual ~AutomationProviderHistoryObserver();

  void HistoryQueryComplete(HistoryService::Handle request_handle,
                            history::QueryResults* results);

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
};

class AutomationProviderImportSettingsObserver
    : public importer::ImporterProgressObserver {
 public:
  AutomationProviderImportSettingsObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message);
  virtual ~AutomationProviderImportSettingsObserver();

  
  virtual void ImportStarted() OVERRIDE;
  virtual void ImportItemStarted(importer::ImportItem item) OVERRIDE;
  virtual void ImportItemEnded(importer::ImportItem item) OVERRIDE;
  virtual void ImportEnded() OVERRIDE;

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
};

class AutomationProviderGetPasswordsObserver : public PasswordStoreConsumer {
 public:
  AutomationProviderGetPasswordsObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message);
  virtual ~AutomationProviderGetPasswordsObserver();

  
  virtual void OnPasswordStoreRequestDone(
      CancelableRequestProvider::Handle handle,
      const std::vector<autofill::PasswordForm*>& result) OVERRIDE;
  virtual void OnGetPasswordStoreResults(
      const std::vector<autofill::PasswordForm*>& results) OVERRIDE;

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
};

class PasswordStoreLoginsChangedObserver
    : public base::RefCountedThreadSafe<
          PasswordStoreLoginsChangedObserver,
          content::BrowserThread::DeleteOnUIThread>,
      public content::NotificationObserver {
 public:
  PasswordStoreLoginsChangedObserver(AutomationProvider* automation,
                                     IPC::Message* reply_message,
                                     PasswordStoreChange::Type expected_type,
                                     const std::string& result_key);

  
  virtual void Init();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  ~PasswordStoreLoginsChangedObserver();
  friend class base::DeleteHelper<PasswordStoreLoginsChangedObserver>;

  
  void RegisterObserversTask();

  
  
  void IndicateDone();

  
  void IndicateError(const std::string& error);

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  scoped_ptr<content::NotificationRegistrar> registrar_;
  PasswordStoreChange::Type expected_type_;
  std::string result_key_;

  
  
  base::WaitableEvent done_event_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreLoginsChangedObserver);
};

class OmniboxAcceptNotificationObserver : public content::NotificationObserver {
 public:
  OmniboxAcceptNotificationObserver(content::NavigationController* controller,
                                    AutomationProvider* automation,
                                    IPC::Message* reply_message);
  virtual ~OmniboxAcceptNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  content::NavigationController* controller_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxAcceptNotificationObserver);
};

class SavePackageNotificationObserver
: public content::DownloadManager::Observer {
 public:
  SavePackageNotificationObserver(content::DownloadManager* download_manager,
                                  AutomationProvider* automation,
                                  IPC::Message* reply_message);
  virtual ~SavePackageNotificationObserver();

  
  virtual void OnSavePackageSuccessfullyFinished(
      content::DownloadManager* manager, content::DownloadItem* item) OVERRIDE;
  virtual void ManagerGoingDown(content::DownloadManager* manager) OVERRIDE;

 private:
  content::DownloadManager* download_manager_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(SavePackageNotificationObserver);
};

class NTPInfoObserver : public content::NotificationObserver {
 public:
  NTPInfoObserver(AutomationProvider* automation, IPC::Message* reply_message);
  virtual ~NTPInfoObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  void OnTopSitesLoaded();
  void OnTopSitesReceived(const history::MostVisitedURLList& visited_list);

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  CancelableRequestProvider::Handle request_;
  scoped_ptr<base::DictionaryValue> ntp_info_;
  history::TopSites* top_sites_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(NTPInfoObserver);
};

class AppLaunchObserver : public content::NotificationObserver {
 public:
  AppLaunchObserver(content::NavigationController* controller,
                    AutomationProvider* automation,
                    IPC::Message* reply_message,
                    extensions::LaunchContainer launch_container);
  virtual ~AppLaunchObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NavigationController* controller_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  content::NotificationRegistrar registrar_;
  extensions::LaunchContainer launch_container_;
  int new_window_id_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchObserver);
};

class GetAllNotificationsObserver : public content::NotificationObserver {
 public:
  GetAllNotificationsObserver(AutomationProvider* automation,
                              IPC::Message* reply_message);
  virtual ~GetAllNotificationsObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  
  void SendMessage();
  
  base::DictionaryValue* NotificationToJson(const Notification* note);

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(GetAllNotificationsObserver);
};

class NewNotificationBalloonObserver : public content::NotificationObserver {
 public:
  NewNotificationBalloonObserver(AutomationProvider* provider,
                                 IPC::Message* reply_message);
  virtual ~NewNotificationBalloonObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NewNotificationBalloonObserver);
};

class OnNotificationBalloonCountObserver
    : public content::NotificationObserver {
 public:
  OnNotificationBalloonCountObserver(AutomationProvider* provider,
                                     IPC::Message* reply_message,
                                     int count);
  virtual ~OnNotificationBalloonCountObserver();

  
  
  
  void CheckBalloonCount();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  BalloonCollection* collection_;
  int count_;

  DISALLOW_COPY_AND_ASSIGN(OnNotificationBalloonCountObserver);
};

class RendererProcessClosedObserver : public content::NotificationObserver {
 public:
  RendererProcessClosedObserver(AutomationProvider* automation,
                                IPC::Message* reply_message);
  virtual ~RendererProcessClosedObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(RendererProcessClosedObserver);
};

class InputEventAckNotificationObserver : public content::NotificationObserver {
 public:
  InputEventAckNotificationObserver(AutomationProvider* automation,
                                    IPC::Message* reply_message,
                                    int event_type, int count);
  virtual ~InputEventAckNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int event_type_;
  int count_;

  DISALLOW_COPY_AND_ASSIGN(InputEventAckNotificationObserver);
};

class NewTabObserver : public content::NotificationObserver {
 public:
  NewTabObserver(AutomationProvider* automation,
                 IPC::Message* reply_message,
                 bool use_json_interface);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  virtual ~NewTabObserver();

  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(NewTabObserver);
};

class WaitForProcessLauncherThreadToGoIdleObserver
    : public base::RefCountedThreadSafe<
          WaitForProcessLauncherThreadToGoIdleObserver,
          content::BrowserThread::DeleteOnUIThread> {
 public:
  WaitForProcessLauncherThreadToGoIdleObserver(
      AutomationProvider* automation, IPC::Message* reply_message);

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<WaitForProcessLauncherThreadToGoIdleObserver>;

  virtual ~WaitForProcessLauncherThreadToGoIdleObserver();

  
  
  
  void RunOnProcessLauncherThread();

  
  
  
  void RunOnProcessLauncherThread2();

  
  void RunOnUIThread();

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(WaitForProcessLauncherThreadToGoIdleObserver);
};

class DragTargetDropAckNotificationObserver
    : public content::NotificationObserver {
 public:
  DragTargetDropAckNotificationObserver(AutomationProvider* automation,
                                        IPC::Message* reply_message);
  virtual ~DragTargetDropAckNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(DragTargetDropAckNotificationObserver);
};

class ProcessInfoObserver : public MemoryDetails {
 public:
  ProcessInfoObserver(AutomationProvider* automation,
                      IPC::Message* reply_message);

  virtual void OnDetailsAvailable() OVERRIDE;

 private:
  virtual ~ProcessInfoObserver();
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(ProcessInfoObserver);
};

class V8HeapStatsObserver : public content::NotificationObserver {
 public:
  V8HeapStatsObserver(AutomationProvider* automation,
                      IPC::Message* reply_message,
                      base::ProcessId renderer_id);
  virtual ~V8HeapStatsObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  base::ProcessId renderer_id_;

  DISALLOW_COPY_AND_ASSIGN(V8HeapStatsObserver);
};

class FPSObserver : public content::NotificationObserver {
 public:
  FPSObserver(AutomationProvider* automation,
              IPC::Message* reply_message,
              base::ProcessId renderer_id,
              int routing_id);
  virtual ~FPSObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  base::ProcessId renderer_id_;
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(FPSObserver);
};

class BrowserOpenedWithNewProfileNotificationObserver
    : public content::NotificationObserver {
 public:
  BrowserOpenedWithNewProfileNotificationObserver(
      AutomationProvider* automation,
      IPC::Message* reply_message);
  virtual ~BrowserOpenedWithNewProfileNotificationObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int new_window_id_;

  DISALLOW_COPY_AND_ASSIGN(BrowserOpenedWithNewProfileNotificationObserver);
};

class ExtensionPopupObserver : public content::NotificationObserver {
 public:
  ExtensionPopupObserver(
      AutomationProvider* automation,
      IPC::Message* reply_message,
      const std::string& extension_id);
  ~ExtensionPopupObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  std::string extension_id_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPopupObserver);
};

#if defined(OS_LINUX)
class WindowMaximizedObserver : public content::NotificationObserver {
 public:
  WindowMaximizedObserver(AutomationProvider* automation,
                          IPC::Message* reply_message);
  virtual ~WindowMaximizedObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(WindowMaximizedObserver);
};
#endif  

class BrowserOpenedWithExistingProfileNotificationObserver
    : public content::NotificationObserver {
 public:
  BrowserOpenedWithExistingProfileNotificationObserver(
      AutomationProvider* automation,
      IPC::Message* reply_message,
      int num_loads);
  virtual ~BrowserOpenedWithExistingProfileNotificationObserver();

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details);
 private:
  content::NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int new_window_id_;
  int num_loads_;

  DISALLOW_COPY_AND_ASSIGN(
      BrowserOpenedWithExistingProfileNotificationObserver);
};

#endif  
