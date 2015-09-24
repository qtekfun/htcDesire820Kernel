// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_TESTING_AUTOMATION_PROVIDER_H_
#define CHROME_BROWSER_AUTOMATION_TESTING_AUTOMATION_PROVIDER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/automation/automation_event_observers.h"
#include "chrome/browser/automation/automation_event_queue.h"
#include "chrome/browser/automation/automation_provider.h"
#include "chrome/browser/automation/automation_provider_json.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/common/page_type.h"
#include "content/public/common/security_style.h"
#include "net/cert/cert_status_flags.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

#if defined(OS_CHROMEOS)
#include "chromeos/dbus/power_manager/power_supply_properties.pb.h"
#include "chromeos/dbus/power_manager_client.h"
#endif

class CreditCard;

namespace base {
class DictionaryValue;
}

namespace content {
class RenderViewHost;
struct NativeWebKeyboardEvent;
struct WebPluginInfo;
}

namespace gfx {
class Rect;
}

class TestingAutomationProvider : public AutomationProvider,
                                  public chrome::BrowserListObserver,
#if defined(OS_CHROMEOS)
                                  public chromeos::PowerManagerClient::Observer,
#endif
                                  public content::NotificationObserver {
 public:
  explicit TestingAutomationProvider(Profile* profile);

  virtual IPC::Channel::Mode GetChannelMode(bool use_named_interface);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

 private:
  virtual ~TestingAutomationProvider();

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

#if defined(OS_CHROMEOS)
  
  virtual void PowerChanged(const power_manager::PowerSupplyProperties& proto);
#endif

  
  void CloseBrowser(int handle, IPC::Message* reply_message);
  void ActivateTab(int handle, int at_index, int* status);
  void AppendTab(int handle, const GURL& url, IPC::Message* reply_message);
  void GetMachPortCount(int* port_count);
  void GetActiveTabIndex(int handle, int* active_tab_index);
  void CloseTab(int tab_handle, bool wait_until_closed,
                IPC::Message* reply_message);
  void GetCookies(const GURL& url, int handle, int* value_size,
                  std::string* value);
  void NavigateToURLBlockUntilNavigationsComplete(int handle, const GURL& url,
                                                  int number_of_navigations,
                                                  IPC::Message* reply_message);
  void NavigationAsync(int handle, const GURL& url, bool* status);
  void Reload(int handle, IPC::Message* reply_message);
  void GetRedirectsFrom(int tab_handle,
                        const GURL& source_url,
                        IPC::Message* reply_message);
  void GetBrowserWindowCount(int* window_count);
  void GetNormalBrowserWindowCount(int* window_count);
  
  
  void GetBrowserWindow(int index, int* handle);
  void ExecuteBrowserCommandAsync(int handle, int command, bool* success);
  void ExecuteBrowserCommand(int handle, int command,
                             IPC::Message* reply_message);
  void TerminateSession(int handle, bool* success);
  void WindowGetViewBounds(int handle, int view_id, bool screen_coordinates,
                           bool* success, gfx::Rect* bounds);
  void SetWindowBounds(int handle, const gfx::Rect& bounds, bool* result);
  void SetWindowVisible(int handle, bool visible, bool* result);
  void GetTabCount(int handle, int* tab_count);
  void GetType(int handle, int* type_as_int);
  void GetTab(int win_handle, int tab_index, int* tab_handle);
  void GetTabTitle(int handle, int* title_string_size, std::wstring* title);
  void GetTabIndex(int handle, int* tabstrip_index);
  void GetTabURL(int handle, bool* success, GURL* url);
  void ExecuteJavascript(int handle,
                         const std::wstring& frame_xpath,
                         const std::wstring& script,
                         IPC::Message* reply_message);

  
  void OpenNewBrowserWindowOfType(int type,
                                  bool show,
                                  IPC::Message* reply_message);

  
  void GetWindowForBrowser(int window_handle, bool* success, int* handle);

  
  
  void GetMetricEventDuration(const std::string& event_name, int* duration_ms);

  
  void BringBrowserToFront(int browser_handle, bool* success);

  
  void HandleOpenFindInPageRequest(const IPC::Message& message,
                                   int handle);

  
  void GetFindWindowVisibility(int handle, bool* visible);

  
  void WaitForBookmarkModelToLoad(int handle, IPC::Message* reply_message);

  
  void BookmarkModelHasLoaded(int handle, bool* loaded);

  
  
  void GetBookmarkBarStatus(base::DictionaryValue* args,
                            IPC::Message* reply_message);

  
  void GetBookmarksAsJSON(base::DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void WaitForBookmarkModelToLoadJSON(base::DictionaryValue* args,
                                      IPC::Message* reply_message);
  void AddBookmark(base::DictionaryValue* args,
                   IPC::Message* reply_message);
  void ReparentBookmark(base::DictionaryValue* args,
                        IPC::Message* reply_message);
  void SetBookmarkTitle(base::DictionaryValue* args,
                        IPC::Message* reply_message);
  void SetBookmarkURL(base::DictionaryValue* args,
                      IPC::Message* reply_message);
  void RemoveBookmark(base::DictionaryValue* args,
                      IPC::Message* reply_message);

  void WaitForBrowserWindowCountToBecome(int target_count,
                                         IPC::Message* reply_message);

  void GoBackBlockUntilNavigationsComplete(int handle,
                                           int number_of_navigations,
                                           IPC::Message* reply_message);

  void GoForwardBlockUntilNavigationsComplete(int handle,
                                              int number_of_navigations,
                                              IPC::Message* reply_message);

  
  
  void SendJSONRequestWithBrowserHandle(int handle,
                                        const std::string& json_request,
                                        IPC::Message* reply_message);
  void SendJSONRequestWithBrowserIndex(int index,
                                       const std::string& json_request,
                                       IPC::Message* reply_message);
  void SendJSONRequest(Browser* browser,
                       const std::string& json_request,
                       IPC::Message* reply_message);

  
  
  typedef void (TestingAutomationProvider::*JsonHandler)(base::DictionaryValue*,
                                                         IPC::Message*);

  
  
  typedef void (TestingAutomationProvider::*BrowserJsonHandler)(
      Browser* browser,
      base::DictionaryValue*,
      IPC::Message*);

  
  static scoped_ptr<DictionaryValue> ParseJSONRequestCommand(
      const std::string& json_request,
      std::string* command,
      std::string* error);
  void BuildJSONHandlerMaps();

  
  
  void SetWindowDimensions(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  
  ListValue* GetInfobarsInfo(content::WebContents* tc);

  
  
  
  
  
  
  
  
  
  void PerformActionOnInfobar(Browser* browser,
                              base::DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  void OpenNewBrowserWindowWithNewProfile(
      base::DictionaryValue* args,
      IPC::Message* reply_message);

  
  
  void OpenNewBrowserWindow(base::DictionaryValue* args,
                            IPC::Message* reply_message);
  
  
  void CloseBrowserWindow(base::DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void GetMultiProfileInfo(
      base::DictionaryValue* args,
      IPC::Message* reply_message);
  
  
  void OpenProfileWindow(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  void GetBrowserInfo(base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  void GetBrowserWindowCountJSON(base::DictionaryValue* args,
                                 IPC::Message* reply_message);

  
  void GetProcessInfo(base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  
  void GetNavigationInfo(Browser* browser,
                         base::DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  
  void GetDownloadsInfo(Browser* browser,
                        base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  void WaitForAllDownloadsToComplete(Browser* browser,
                                     base::DictionaryValue* args,
                                     IPC::Message* reply_message);

  
  
  void PerformActionOnDownload(Browser* browser,
                               base::DictionaryValue* args,
                               IPC::Message* reply_message);

  
  
  void GetHistoryInfo(Browser* browser,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void LoadSearchEngineInfo(Browser* browser,
                            base::DictionaryValue* args,
                            IPC::Message* reply_message);

  
  
  
  
  
  
  void SetDownloadShelfVisibleJSON(base::DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  
  
  void IsDownloadShelfVisibleJSON(base::DictionaryValue* args,
                                  IPC::Message* reply_message);

  
  
  
  
  
  
  void GetDownloadDirectoryJSON(base::DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  
  void GetSearchEngineInfo(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  void AddOrEditSearchEngine(Browser* browser,
                             base::DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  
  void PerformActionOnSearchEngine(Browser* browser,
                                   base::DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  void GetLocalStatePrefsInfo(base::DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  void SetLocalStatePrefs(base::DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void GetPrefsInfo(base::DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void SetPrefs(base::DictionaryValue* args,
                IPC::Message* reply_message);

  
  
  
  
  
  void GetInitialLoadTimes(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void GetPluginsInfo(Browser* browser,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);
  void GetPluginsInfoCallback(Browser* browser,
      base::DictionaryValue* args,
      IPC::Message* reply_message,
      const std::vector<content::WebPluginInfo>& plugins);

  
  
  void EnablePlugin(Browser* browser,
                    base::DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void DisablePlugin(Browser* browser,
                     base::DictionaryValue* args,
                     IPC::Message* reply_message);

  
  
  
  
  void GetOmniboxInfo(Browser* browser,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void SetOmniboxText(Browser* browser,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void OmniboxMovePopupSelection(Browser* browser,
                                 base::DictionaryValue* args,
                                 IPC::Message* reply_message);

  
  
  
  
  void OmniboxAcceptInput(Browser* browser,
                          base::DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void SaveTabContents(Browser* browser,
                       base::DictionaryValue* args,
                       IPC::Message* reply_message);

  
  
  
  
  void AddSavedPassword(Browser* browser,
                        base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  
  void RemoveSavedPassword(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void GetSavedPasswords(Browser* browser,
                         base::DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void InstallExtension(base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  void GetExtensionsInfo(base::DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void UninstallExtensionById(base::DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  
  
  void SetExtensionStateById(base::DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  void TriggerPageActionById(base::DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  void TriggerBrowserActionById(base::DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  void UpdateExtensionsNow(base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void OverrideGeoposition(base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  void FindInPage(Browser* browser,
                  base::DictionaryValue* args,
                  IPC::Message* reply_message);

  
  
  
  
  void OpenFindInPage(base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  
  
  void IsFindInPageVisible(base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void GetAllNotifications(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void CloseNotification(Browser* browser,
                         base::DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void WaitForNotificationCount(Browser* browser,
                                base::DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  void GetNTPInfo(Browser* browser,
                  base::DictionaryValue* args,
                  IPC::Message* reply_message);

  
  
  void RemoveNTPMostVisitedThumbnail(Browser* browser,
                                     base::DictionaryValue* args,
                                     IPC::Message* reply_message);

  
  
  
  void RestoreAllNTPMostVisitedThumbnails(Browser* browser,
                                          base::DictionaryValue* args,
                                          IPC::Message* reply_message);

  
  
  void KillRendererProcess(Browser* browser,
                           base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  bool BuildWebKeyEventFromArgs(base::DictionaryValue* args,
                                std::string* error,
                                content::NativeWebKeyboardEvent* event);

  
  void LaunchApp(Browser* browser,
                 base::DictionaryValue* args,
                 IPC::Message* reply_message);

  
  void SetAppLaunchType(Browser* browser,
                        base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  void GetV8HeapStats(Browser* browser,
                      base::DictionaryValue* args,
                      IPC::Message* reply_message);

  
  void GetFPS(Browser* browser,
              base::DictionaryValue* args,
              IPC::Message* reply_message);

  
  void IsFullscreenForBrowser(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void IsFullscreenForTab(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void IsMouseLocked(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void IsMouseLockPermissionRequested(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void IsFullscreenPermissionRequested(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void IsFullscreenBubbleDisplayed(Browser* browser,
              base::DictionaryValue* args,
              IPC::Message* reply_message);
  void IsFullscreenBubbleDisplayingButtons(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void AcceptCurrentFullscreenOrMouseLockRequest(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);
  void DenyCurrentFullscreenOrMouseLockRequest(Browser* browser,
            base::DictionaryValue* args,
            IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void GetIndicesFromTab(base::DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  
  
  
  
  void ExecuteBrowserCommandAsyncJSON(DictionaryValue* args,
                                      IPC::Message* reply_message);

  
  
  
  
  
  
  void ExecuteBrowserCommandJSON(DictionaryValue* args,
                                 IPC::Message* reply_message);

  
  
  
  
  
  
  void IsMenuCommandEnabledJSON(DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  
  
  
  
  
  void GetTabInfo(DictionaryValue* args,
                  IPC::Message* reply_message);

  
  
  
  
  void GetTabCountJSON(DictionaryValue* args,
                       IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  void NavigateToURL(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  void GetActiveTabIndexJSON(DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  void AppendTabJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void WaitUntilNavigationCompletes(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExecuteJavascriptJSON(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  void AddDomEventObserver(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void RemoveEventObserver(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  void GetNextEvent(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void ClearEventQueue(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExecuteJavascriptInRenderView(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  void GoForward(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  void GoBack(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void ReloadJSON(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void GetCookiesJSON(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  void DeleteCookieJSON(base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetCookieJSON(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  void GetCookiesInBrowserContext(base::DictionaryValue* args,
                                  IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void DeleteCookieInBrowserContext(base::DictionaryValue* args,
                                    IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  void SetCookieInBrowserContext(base::DictionaryValue* args,
                                 IPC::Message* reply_message);

  
  
  
  
  void GetTabIds(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void IsTabIdValid(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void CloseTabJSON(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetViewBounds(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  void MaximizeView(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseClick(base::DictionaryValue* args,
                        IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseMove(base::DictionaryValue* args,
                       IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseDrag(base::DictionaryValue* args,
                       IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseButtonDown(base::DictionaryValue* args,
                             IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseButtonUp(base::DictionaryValue* args,
                           IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  void WebkitMouseDoubleClick(base::DictionaryValue* args,
                              IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void DragAndDropFilePaths(base::DictionaryValue* args,
                            IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SendWebkitKeyEvent(base::DictionaryValue* args,
                          IPC::Message* message);

  
  
  
  
  void GetAppModalDialogMessage(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  void AcceptOrDismissAppModalDialog(
      base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void ActivateTabJSON(base::DictionaryValue* args, IPC::Message* message);

  
  
  void WaitForTabToBeRestored(DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  void ActionOnSSLBlockingPage(DictionaryValue* args,
                               IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void GetSecurityState(DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  
  
  void BringBrowserToFrontJSON(base::DictionaryValue* args,
                               IPC::Message* message);

  
  
  
  
  
  
  void IsPageActionVisible(base::DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  
  
  void CreateNewAutomationProvider(base::DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  
  
  
  void RefreshPolicies(base::DictionaryValue* args,
                       IPC::Message* reply_message);

  
  
  
  
  
  void SimulateAsanMemoryBug(base::DictionaryValue* args,
                             IPC::Message* reply_message);

#if defined(OS_CHROMEOS)
  

  
  
  
  
  void AcceptOOBENetworkScreen(base::DictionaryValue* args,
                               IPC::Message* reply_message);

  
  
  
  
  void AcceptOOBEEula(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void CancelOOBEUpdate(base::DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  
  
  
  void PickUserImage(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  void SkipToLogin(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  void GetOOBEScreenInfo(DictionaryValue* args, IPC::Message* reply_message);

  
  void GetLoginInfo(base::DictionaryValue* args, IPC::Message* reply_message);

  void ShowCreateAccountUI(base::DictionaryValue* args,
                           IPC::Message* reply_message);

  void LoginAsGuest(base::DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  void SubmitLoginForm(base::DictionaryValue* args,
                       IPC::Message* reply_message);

  void AddLoginEventObserver(DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void ExecuteJavascriptInOOBEWebUI(
      base::DictionaryValue* args, IPC::Message* reply_message);

  void SignOut(base::DictionaryValue* args, IPC::Message* reply_message);

  
  void LockScreen(base::DictionaryValue* args, IPC::Message* reply_message);

  void UnlockScreen(base::DictionaryValue* args, IPC::Message* reply_message);

  void SignoutInScreenLocker(base::DictionaryValue* args,
                             IPC::Message* reply_message);

  
  void GetBatteryInfo(base::DictionaryValue* args, IPC::Message* reply_message);

  
  void EnableSpokenFeedback(DictionaryValue* args, IPC::Message* reply_message);

  void IsSpokenFeedbackEnabled(DictionaryValue* args,
                               IPC::Message* reply_message);

  
  void GetTimeInfo(Browser* browser, base::DictionaryValue* args,
                   IPC::Message* reply_message);

  void GetTimeInfo(base::DictionaryValue* args, IPC::Message* reply_message);

  void SetTimezone(base::DictionaryValue* args, IPC::Message* reply_message);

  void UpdateCheck(base::DictionaryValue* args, IPC::Message* reply_message);

  
  void GetVolumeInfo(base::DictionaryValue* args, IPC::Message* reply_message);

  void SetVolume(base::DictionaryValue* args, IPC::Message* reply_message);

  void SetMute(base::DictionaryValue* args, IPC::Message* reply_message);

  void CaptureProfilePhoto(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  void OpenCrosh(base::DictionaryValue* args, IPC::Message* reply_message);

  void AddChromeosObservers();
  void RemoveChromeosObservers();

#endif  

  void WaitForTabCountToBecome(int browser_handle,
                               int target_tab_count,
                               IPC::Message* reply_message);

  void WaitForInfoBarCount(int tab_handle,
                           size_t target_count,
                           IPC::Message* reply_message);

  void WaitForProcessLauncherThreadToGoIdle(IPC::Message* reply_message);

  void OnRemoveProvider();  

  
  void ExecuteJavascriptInRenderViewFrame(
      const base::string16& frame_xpath, const base::string16& script,
      IPC::Message* reply_message, content::RenderViewHost* render_view_host);

  
  void EnsureTabSelected(Browser* browser, content::WebContents* tab);

#if defined(OS_CHROMEOS)
  power_manager::PowerSupplyProperties power_supply_properties_;
#endif  

  std::map<std::string, JsonHandler> handler_map_;
  std::map<std::string, BrowserJsonHandler> browser_handler_map_;

  content::NotificationRegistrar registrar_;

  
  
  scoped_ptr<AutomationEventQueue> automation_event_queue_;

  
  
  static const int kSynchronousCommands[];

  DISALLOW_COPY_AND_ASSIGN(TestingAutomationProvider);
};

#endif  
