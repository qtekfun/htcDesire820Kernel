// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_OBSERVERS_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_PROVIDER_OBSERVERS_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/automation/automation_provider_json.h"
#include "chrome/browser/automation/automation_tab_helper.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/browsing_data_remover.h"
#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/cros/network_library.h"
#endif  
#include "chrome/browser/download/download_item.h"
#include "chrome/browser/download/download_manager.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/importer/importer_data_types.h"
#include "chrome/browser/importer/importer_progress_observer.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "chrome/browser/tabs/tab_strip_model.h"
#include "chrome/common/automation_constants.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"
#include "ui/gfx/size.h"

class AutocompleteEditModel;
class AutomationProvider;
class BalloonCollection;
class Browser;
class Extension;
class ExtensionProcessManager;
class NavigationController;
class RenderViewHost;
class SavePackage;
class TabContents;
class TranslateInfoBarDelegate;

namespace history {
class TopSites;
}

namespace IPC {
class Message;
}

class InitialLoadObserver : public NotificationObserver {
 public:
  InitialLoadObserver(size_t tab_count, AutomationProvider* automation);
  virtual ~InitialLoadObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  
  
  
  DictionaryValue* GetTimingInformation() const;

 private:
  class TabTime;
  typedef std::map<uintptr_t, TabTime> TabTimeMap;
  typedef std::set<uintptr_t> TabSet;

  void ConditionMet();

  NotificationRegistrar registrar_;

  base::WeakPtr<AutomationProvider> automation_;
  size_t outstanding_tab_count_;
  base::TimeTicks init_time_;
  TabTimeMap loading_tabs_;
  TabSet finished_tabs_;

  DISALLOW_COPY_AND_ASSIGN(InitialLoadObserver);
};

#if defined(OS_CHROMEOS)
class NetworkManagerInitObserver
    : public chromeos::NetworkLibrary::NetworkManagerObserver {
 public:
  explicit NetworkManagerInitObserver(AutomationProvider* automation);
  virtual ~NetworkManagerInitObserver();
  virtual bool Init();
  virtual void OnNetworkManagerChanged(chromeos::NetworkLibrary* obj);

 private:
  base::WeakPtr<AutomationProvider> automation_;

  DISALLOW_COPY_AND_ASSIGN(NetworkManagerInitObserver);
};
#endif  

class NewTabUILoadObserver : public NotificationObserver {
 public:
  explicit NewTabUILoadObserver(AutomationProvider* automation);
  virtual ~NewTabUILoadObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;

  DISALLOW_COPY_AND_ASSIGN(NewTabUILoadObserver);
};

class NavigationControllerRestoredObserver : public NotificationObserver {
 public:
  NavigationControllerRestoredObserver(AutomationProvider* automation,
                                       NavigationController* controller,
                                       IPC::Message* reply_message);
  virtual ~NavigationControllerRestoredObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  bool FinishedRestoring();
  void SendDone();

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  NavigationController* controller_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NavigationControllerRestoredObserver);
};

class NavigationNotificationObserver : public NotificationObserver {
 public:
  NavigationNotificationObserver(NavigationController* controller,
                                 AutomationProvider* automation,
                                 IPC::Message* reply_message,
                                 int number_of_navigations,
                                 bool include_current_navigation,
                                 bool use_json_interface);
  virtual ~NavigationNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  void ConditionMet(AutomationMsg_NavigationResponseValues navigation_result);

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  NavigationController* controller_;
  int navigations_remaining_;
  bool navigation_started_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(NavigationNotificationObserver);
};

class TabStripNotificationObserver : public NotificationObserver {
 public:
  TabStripNotificationObserver(NotificationType notification,
                               AutomationProvider* automation);
  virtual ~TabStripNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  virtual void ObserveTab(NavigationController* controller) = 0;

 protected:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  NotificationType notification_;
};

class TabAppendedNotificationObserver : public TabStripNotificationObserver {
 public:
  TabAppendedNotificationObserver(Browser* parent,
                                  AutomationProvider* automation,
                                  IPC::Message* reply_message);
  virtual ~TabAppendedNotificationObserver();

  virtual void ObserveTab(NavigationController* controller);

 protected:
  Browser* parent_;
  scoped_ptr<IPC::Message> reply_message_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TabAppendedNotificationObserver);
};

class TabClosedNotificationObserver : public TabStripNotificationObserver {
 public:
  TabClosedNotificationObserver(AutomationProvider* automation,
                                bool wait_until_closed,
                                IPC::Message* reply_message);
  virtual ~TabClosedNotificationObserver();

  virtual void ObserveTab(NavigationController* controller);

  void set_for_browser_command(bool for_browser_command);

 protected:
  scoped_ptr<IPC::Message> reply_message_;
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
  
  virtual void TabInsertedAt(TabContentsWrapper* contents,
                             int index,
                             bool foreground);
  virtual void TabDetachedAt(TabContentsWrapper* contents, int index);
  virtual void TabStripModelDeleted();

 private:
  virtual ~TabCountChangeObserver();

  
  
  void CheckTabCount();

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  TabStripModel* tab_strip_model_;

  const int target_tab_count_;

  DISALLOW_COPY_AND_ASSIGN(TabCountChangeObserver);
};

class ExtensionInstallNotificationObserver : public NotificationObserver {
 public:
  ExtensionInstallNotificationObserver(AutomationProvider* automation,
                                       int id,
                                       IPC::Message* reply_message);
  virtual ~ExtensionInstallNotificationObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  void SendResponse(AutomationMsg_ExtensionResponseValues response);

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  int id_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstallNotificationObserver);
};

class ExtensionUninstallObserver : public NotificationObserver {
 public:
  ExtensionUninstallObserver(AutomationProvider* automation,
                             IPC::Message* reply_message,
                             const std::string& id);
  virtual ~ExtensionUninstallObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  std::string id_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUninstallObserver);
};

class ExtensionReadyNotificationObserver : public NotificationObserver {
 public:
  ExtensionReadyNotificationObserver(ExtensionProcessManager* manager,
                                     AutomationProvider* automation,
                                     int id,
                                     IPC::Message* reply_message);
  virtual ~ExtensionReadyNotificationObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  ExtensionProcessManager* manager_;
  base::WeakPtr<AutomationProvider> automation_;
  int id_;
  scoped_ptr<IPC::Message> reply_message_;
  const Extension* extension_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionReadyNotificationObserver);
};

class ExtensionUnloadNotificationObserver : public NotificationObserver {
 public:
  ExtensionUnloadNotificationObserver();
  virtual ~ExtensionUnloadNotificationObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  bool did_receive_unload_notification() {
    return did_receive_unload_notification_;
  }

 private:
  NotificationRegistrar registrar_;
  bool did_receive_unload_notification_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUnloadNotificationObserver);
};

class ExtensionsUpdatedObserver : public NotificationObserver {
 public:
  ExtensionsUpdatedObserver(ExtensionProcessManager* manager,
                            AutomationProvider* automation,
                            IPC::Message* reply_message);
  virtual ~ExtensionsUpdatedObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  ExtensionProcessManager* manager_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  std::set<std::string> in_progress_updates_;
  bool updater_finished_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionsUpdatedObserver);
};

class ExtensionTestResultNotificationObserver : public NotificationObserver {
 public:
  explicit ExtensionTestResultNotificationObserver(
      AutomationProvider* automation);
  virtual ~ExtensionTestResultNotificationObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void MaybeSendResult();

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  
  
  
  
  
  
  
  std::deque<bool> results_;
  std::deque<std::string> messages_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionTestResultNotificationObserver);
};

class BrowserOpenedNotificationObserver : public NotificationObserver {
 public:
  BrowserOpenedNotificationObserver(AutomationProvider* automation,
                                    IPC::Message* reply_message);
  virtual ~BrowserOpenedNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void set_for_browser_command(bool for_browser_command);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int new_window_id_;
  bool for_browser_command_;

  DISALLOW_COPY_AND_ASSIGN(BrowserOpenedNotificationObserver);
};

class BrowserClosedNotificationObserver : public NotificationObserver {
 public:
  BrowserClosedNotificationObserver(Browser* browser,
                                    AutomationProvider* automation,
                                    IPC::Message* reply_message);
  virtual ~BrowserClosedNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void set_for_browser_command(bool for_browser_command);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool for_browser_command_;

  DISALLOW_COPY_AND_ASSIGN(BrowserClosedNotificationObserver);
};

class BrowserCountChangeNotificationObserver : public NotificationObserver {
 public:
  BrowserCountChangeNotificationObserver(int target_count,
                                         AutomationProvider* automation,
                                         IPC::Message* reply_message);
  virtual ~BrowserCountChangeNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  int target_count_;
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(BrowserCountChangeNotificationObserver);
};

class AppModalDialogShownObserver : public NotificationObserver {
 public:
  AppModalDialogShownObserver(AutomationProvider* automation,
                              IPC::Message* reply_message);
  virtual ~AppModalDialogShownObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(AppModalDialogShownObserver);
};

class ExecuteBrowserCommandObserver : public NotificationObserver {
 public:
  virtual ~ExecuteBrowserCommandObserver();

  static bool CreateAndRegisterObserver(AutomationProvider* automation,
                                        Browser* browser,
                                        int command,
                                        IPC::Message* reply_message);

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  ExecuteBrowserCommandObserver(AutomationProvider* automation,
                                IPC::Message* reply_message);

  bool Register(int command);

  bool GetNotificationType(int command, NotificationType::Type* type);

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  NotificationType::Type notification_type_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(ExecuteBrowserCommandObserver);
};

class FindInPageNotificationObserver : public NotificationObserver {
 public:
  FindInPageNotificationObserver(AutomationProvider* automation,
                                 TabContents* parent_tab,
                                 bool reply_with_json,
                                 IPC::Message* reply_message);
  virtual ~FindInPageNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  
  
  
  
  static const int kFindInPageRequestId;

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  
  
  int active_match_ordinal_;
  
  bool reply_with_json_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(FindInPageNotificationObserver);
};

class DomOperationObserver : public NotificationObserver {
 public:
  DomOperationObserver();
  virtual ~DomOperationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  virtual void OnDomOperationCompleted(const std::string& json) = 0;

 private:
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(DomOperationObserver);
};

class DomOperationMessageSender : public DomOperationObserver {
 public:
  explicit DomOperationMessageSender(AutomationProvider* automation,
                                     IPC::Message* relpy_message,
                                     bool use_json_interface);
  virtual ~DomOperationMessageSender();

  virtual void OnDomOperationCompleted(const std::string& json);

 private:
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  bool use_json_interface_;

  DISALLOW_COPY_AND_ASSIGN(DomOperationMessageSender);
};

class DocumentPrintedNotificationObserver : public NotificationObserver {
 public:
  DocumentPrintedNotificationObserver(AutomationProvider* automation,
                                      IPC::Message* reply_message);
  virtual ~DocumentPrintedNotificationObserver();

  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  bool success_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(DocumentPrintedNotificationObserver);
};

class MetricEventDurationObserver : public NotificationObserver {
 public:
  MetricEventDurationObserver();
  virtual ~MetricEventDurationObserver();

  
  int GetEventDurationMs(const std::string& event_name);

  
  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;

  typedef std::map<std::string, int> EventDurationMap;
  EventDurationMap durations_;

  DISALLOW_COPY_AND_ASSIGN(MetricEventDurationObserver);
};

class PageTranslatedObserver : public NotificationObserver {
 public:
  PageTranslatedObserver(AutomationProvider* automation,
                         IPC::Message* reply_message,
                         TabContents* tab_contents);
  virtual ~PageTranslatedObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(PageTranslatedObserver);
};

class TabLanguageDeterminedObserver : public NotificationObserver {
 public:
  TabLanguageDeterminedObserver(AutomationProvider* automation,
                                IPC::Message* reply_message,
                                TabContents* tab_contents,
                                TranslateInfoBarDelegate* translate_bar);
  virtual ~TabLanguageDeterminedObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  TabContents* tab_contents_;
  TranslateInfoBarDelegate* translate_bar_;

  DISALLOW_COPY_AND_ASSIGN(TabLanguageDeterminedObserver);
};

class InfoBarCountObserver : public NotificationObserver {
 public:
  InfoBarCountObserver(AutomationProvider* automation,
                       IPC::Message* reply_message,
                       TabContents* tab_contents,
                       size_t target_count);
  virtual ~InfoBarCountObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  
  void CheckCount();

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  TabContents* tab_contents_;

  const size_t target_count_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarCountObserver);
};

#if defined(OS_CHROMEOS)
class LoginManagerObserver : public NotificationObserver {
 public:
  LoginManagerObserver(AutomationProvider* automation,
                       IPC::Message* reply_message);
  virtual ~LoginManagerObserver();

  
  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(LoginManagerObserver);
};

class ScreenLockUnlockObserver : public NotificationObserver {
 public:
  
  
  ScreenLockUnlockObserver(AutomationProvider* automation,
                           IPC::Message* reply_message,
                           bool lock_screen);
  virtual ~ScreenLockUnlockObserver();

  
  virtual void Observe(NotificationType type, const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  AutomationProvider* automation_;
  IPC::Message* reply_message_;
  bool lock_screen_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLockUnlockObserver);
};

class NetworkScanObserver
    : public chromeos::NetworkLibrary::NetworkManagerObserver {
 public:
  NetworkScanObserver(AutomationProvider* automation,
                      IPC::Message* reply_message);

  virtual ~NetworkScanObserver();

  
  virtual void OnNetworkManagerChanged(chromeos::NetworkLibrary* obj);

 private:
  AutomationProvider* automation_;
  IPC::Message* reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NetworkScanObserver);
};

class NetworkConnectObserver
    : public chromeos::NetworkLibrary::NetworkManagerObserver {
 public:
  NetworkConnectObserver(AutomationProvider* automation,
                         IPC::Message* reply_message);

  virtual ~NetworkConnectObserver();

  virtual const chromeos::WifiNetwork* GetWifiNetwork(
      chromeos::NetworkLibrary* network_library) = 0;

  
  virtual void OnNetworkManagerChanged(chromeos::NetworkLibrary* obj);

 private:
  AutomationProvider* automation_;
  IPC::Message* reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConnectObserver);
};

class ServicePathConnectObserver : public NetworkConnectObserver {
 public:
  ServicePathConnectObserver(AutomationProvider* automation,
                             IPC::Message* reply_message,
                             const std::string& service_path);

  virtual const chromeos::WifiNetwork* GetWifiNetwork(
      chromeos::NetworkLibrary* network_library);

 private:
  std::string service_path_;

  DISALLOW_COPY_AND_ASSIGN(ServicePathConnectObserver);
};

class SSIDConnectObserver : public NetworkConnectObserver {
 public:
  SSIDConnectObserver(AutomationProvider* automation,
                      IPC::Message* reply_message,
                      const std::string& ssid);

  virtual const chromeos::WifiNetwork* GetWifiNetwork(
      chromeos::NetworkLibrary* network_library);

 private:
  std::string ssid_;

  DISALLOW_COPY_AND_ASSIGN(SSIDConnectObserver);
};
#endif  

class AutomationProviderBookmarkModelObserver : BookmarkModelObserver {
 public:
  AutomationProviderBookmarkModelObserver(AutomationProvider* provider,
                                          IPC::Message* reply_message,
                                          BookmarkModel* model);
  virtual ~AutomationProviderBookmarkModelObserver();

  virtual void Loaded(BookmarkModel* model);
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) {}
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) {}
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) {}
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) {}
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node) {}
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) {}

 private:
  
  
  
  void ReplyAndDelete(bool success);

  base::WeakPtr<AutomationProvider> automation_provider_;
  scoped_ptr<IPC::Message> reply_message_;
  BookmarkModel* model_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderBookmarkModelObserver);
};

class AutomationProviderDownloadItemObserver : public DownloadItem::Observer {
 public:
  AutomationProviderDownloadItemObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      int downloads);
  virtual ~AutomationProviderDownloadItemObserver();

  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download);

 private:
  void RemoveAndCleanupOnLastEntry(DownloadItem* download);

  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  int downloads_;
  bool interrupted_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderDownloadItemObserver);
};

class AutomationProviderDownloadUpdatedObserver
    : public DownloadItem::Observer {
 public:
  AutomationProviderDownloadUpdatedObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      bool wait_for_open);
  virtual ~AutomationProviderDownloadUpdatedObserver();

  virtual void OnDownloadUpdated(DownloadItem* download);
  virtual void OnDownloadOpened(DownloadItem* download);

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  bool wait_for_open_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderDownloadUpdatedObserver);
};

class AutomationProviderDownloadModelChangedObserver
    : public DownloadManager::Observer {
 public:
  AutomationProviderDownloadModelChangedObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message,
      DownloadManager* download_manager);
  virtual ~AutomationProviderDownloadModelChangedObserver();

  virtual void ModelChanged();

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
  DownloadManager* download_manager_;

  DISALLOW_COPY_AND_ASSIGN(AutomationProviderDownloadModelChangedObserver);
};

class AutomationProviderSearchEngineObserver
    : public TemplateURLModelObserver {
 public:
  AutomationProviderSearchEngineObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message);
  virtual ~AutomationProviderSearchEngineObserver();

  virtual void OnTemplateURLModelChanged();

 private:
  base::WeakPtr<AutomationProvider> provider_;
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
      const std::vector<webkit_glue::PasswordForm*>& result);

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
};

class AutomationProviderBrowsingDataObserver
    : public BrowsingDataRemover::Observer {
 public:
  AutomationProviderBrowsingDataObserver(
      AutomationProvider* provider,
      IPC::Message* reply_message);
  virtual ~AutomationProviderBrowsingDataObserver();

  virtual void OnBrowsingDataRemoverDone();

 private:
  base::WeakPtr<AutomationProvider> provider_;
  scoped_ptr<IPC::Message> reply_message_;
};

class OmniboxAcceptNotificationObserver : public NotificationObserver {
 public:
  OmniboxAcceptNotificationObserver(NavigationController* controller,
                                 AutomationProvider* automation,
                                 IPC::Message* reply_message);
  virtual ~OmniboxAcceptNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  NavigationController* controller_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxAcceptNotificationObserver);
};

class SavePackageNotificationObserver : public NotificationObserver {
 public:
  SavePackageNotificationObserver(SavePackage* save_package,
                                  AutomationProvider* automation,
                                  IPC::Message* reply_message);
  virtual ~SavePackageNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(SavePackageNotificationObserver);
};

class PageSnapshotTaker : public DomOperationObserver {
 public:
  PageSnapshotTaker(AutomationProvider* automation,
                    IPC::Message* reply_message,
                    RenderViewHost* render_view,
                    const FilePath& path);
  virtual ~PageSnapshotTaker();

  
  void Start();

 private:
  
  virtual void OnDomOperationCompleted(const std::string& json);

  
  
  void OnSnapshotTaken(const SkBitmap& bitmap);

  
  void ExecuteScript(const std::wstring& javascript);

  
  void SendMessage(bool success);

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  RenderViewHost* render_view_;
  FilePath image_path_;
  bool received_width_;
  gfx::Size entire_page_size_;

  DISALLOW_COPY_AND_ASSIGN(PageSnapshotTaker);
};

class NTPInfoObserver : public NotificationObserver {
 public:
  NTPInfoObserver(AutomationProvider* automation,
                  IPC::Message* reply_message,
                  CancelableRequestConsumer* consumer);
  virtual ~NTPInfoObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  void OnTopSitesLoaded();
  void OnTopSitesReceived(const history::MostVisitedURLList& visited_list);

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  CancelableRequestConsumer* consumer_;
  CancelableRequestProvider::Handle request_;
  scoped_ptr<DictionaryValue> ntp_info_;
  history::TopSites* top_sites_;
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(NTPInfoObserver);
};

class AppLaunchObserver : public NotificationObserver {
 public:
  AppLaunchObserver(NavigationController* controller,
                    AutomationProvider* automation,
                    IPC::Message* reply_message,
                    extension_misc::LaunchContainer launch_container);
  virtual ~AppLaunchObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NavigationController* controller_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  NotificationRegistrar registrar_;
  extension_misc::LaunchContainer launch_container_;
  int new_window_id_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchObserver);
};

class AutocompleteEditFocusedObserver : public NotificationObserver {
 public:
  AutocompleteEditFocusedObserver(AutomationProvider* automation,
                                  AutocompleteEditModel* autocomplete_edit,
                                  IPC::Message* reply_message);
  virtual ~AutocompleteEditFocusedObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  AutocompleteEditModel* autocomplete_edit_model_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteEditFocusedObserver);
};

class GetActiveNotificationsObserver : public NotificationObserver {
 public:
  GetActiveNotificationsObserver(AutomationProvider* automation,
                                 IPC::Message* reply_message);
  virtual ~GetActiveNotificationsObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  void SendMessage();

  AutomationJSONReply reply_;
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(GetActiveNotificationsObserver);
};

class OnNotificationBalloonCountObserver {
 public:
  OnNotificationBalloonCountObserver(AutomationProvider* provider,
                                     IPC::Message* reply_message,
                                     BalloonCollection* collection,
                                     int count);

  void OnBalloonCollectionChanged();

 private:
  AutomationJSONReply reply_;
  BalloonCollection* collection_;
  int count_;

  DISALLOW_COPY_AND_ASSIGN(OnNotificationBalloonCountObserver);
};

class RendererProcessClosedObserver : public NotificationObserver {
 public:
  RendererProcessClosedObserver(AutomationProvider* automation,
                                IPC::Message* reply_message);
  virtual ~RendererProcessClosedObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(RendererProcessClosedObserver);
};

class InputEventAckNotificationObserver : public NotificationObserver {
 public:
  InputEventAckNotificationObserver(AutomationProvider* automation,
                                    IPC::Message* reply_message,
                                    int event_type);
  virtual ~InputEventAckNotificationObserver();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;
  int event_type_;

  DISALLOW_COPY_AND_ASSIGN(InputEventAckNotificationObserver);
};

class AllTabsStoppedLoadingObserver : public TabEventObserver {
 public:
  AllTabsStoppedLoadingObserver(AutomationProvider* automation,
                                IPC::Message* reply_message);
  virtual ~AllTabsStoppedLoadingObserver();

  
  virtual void OnFirstPendingLoad(TabContents* tab_contents);
  virtual void OnNoMorePendingLoads(TabContents* tab_contents);

 private:
  typedef std::set<TabContents*> TabSet;

  
  
  void CheckIfNoMorePendingLoads();

  TabSet pending_tabs_;
  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(AllTabsStoppedLoadingObserver);
};

class NewTabObserver : public NotificationObserver {
 public:
  NewTabObserver(AutomationProvider* automation, IPC::Message* reply_message);

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

 private:
  virtual ~NewTabObserver();

  NotificationRegistrar registrar_;
  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(NewTabObserver);
};

class WaitForProcessLauncherThreadToGoIdleObserver
    : public base::RefCountedThreadSafe<
          WaitForProcessLauncherThreadToGoIdleObserver,
          BrowserThread::DeleteOnUIThread> {
 public:
  WaitForProcessLauncherThreadToGoIdleObserver(
      AutomationProvider* automation, IPC::Message* reply_message);

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<WaitForProcessLauncherThreadToGoIdleObserver>;

  virtual ~WaitForProcessLauncherThreadToGoIdleObserver();

  
  
  
  void RunOnProcessLauncherThread();

  
  
  
  void RunOnProcessLauncherThread2();

  
  void RunOnUIThread();

  base::WeakPtr<AutomationProvider> automation_;
  scoped_ptr<IPC::Message> reply_message_;

  DISALLOW_COPY_AND_ASSIGN(WaitForProcessLauncherThreadToGoIdleObserver);
};

#endif  
