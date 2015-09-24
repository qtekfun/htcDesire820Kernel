// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_TESTING_AUTOMATION_PROVIDER_H_
#define CHROME_BROWSER_AUTOMATION_TESTING_AUTOMATION_PROVIDER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/automation/automation_provider.h"
#include "chrome/browser/automation/automation_provider_json.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/importer/importer_list.h"
#include "chrome/browser/sync/profile_sync_service_harness.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_type.h"

class DictionaryValue;
class TemplateURLModel;

class TestingAutomationProvider : public AutomationProvider,
                                  public BrowserList::Observer,
                                  public ImporterList::Observer,
                                  public NotificationObserver {
 public:
  explicit TestingAutomationProvider(Profile* profile);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

 private:
  class PopupMenuWaiter;

  
  struct ImportSettingsData {
    string16 browser_name;
    int import_items;
    bool first_run;
    Browser* browser;
    IPC::Message* reply_message;
  };

  virtual ~TestingAutomationProvider();

  
  virtual void OnBrowserAdded(const Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(const Browser* browser) OVERRIDE;

  
  virtual void OnSourceProfilesLoaded() OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void CloseBrowser(int handle, IPC::Message* reply_message);
  void CloseBrowserAsync(int browser_handle);
  void ActivateTab(int handle, int at_index, int* status);
  void AppendTab(int handle, const GURL& url, IPC::Message* reply_message);
  void GetActiveTabIndex(int handle, int* active_tab_index);
  void CloseTab(int tab_handle, bool wait_until_closed,
                IPC::Message* reply_message);
  void GetCookies(const GURL& url, int handle, int* value_size,
                  std::string* value);
  void SetCookie(const GURL& url,
                 const std::string& value,
                 int handle,
                 int* response_value);
  void DeleteCookie(const GURL& url, const std::string& cookie_name,
                    int handle, bool* success);
  void ShowCollectedCookiesDialog(int handle, bool* success);
  void NavigateToURLBlockUntilNavigationsComplete(int handle, const GURL& url,
                                                  int number_of_navigations,
                                                  IPC::Message* reply_message);
  void NavigationAsync(int handle, const GURL& url, bool* status);
  void NavigationAsyncWithDisposition(int handle,
                                      const GURL& url,
                                      WindowOpenDisposition disposition,
                                      bool* status);
  void Reload(int handle, IPC::Message* reply_message);
  void SetAuth(int tab_handle, const std::wstring& username,
               const std::wstring& password, IPC::Message* reply_message);
  void CancelAuth(int tab_handle, IPC::Message* reply_message);
  void NeedsAuth(int tab_handle, bool* needs_auth);
  void GetRedirectsFrom(int tab_handle,
                        const GURL& source_url,
                        IPC::Message* reply_message);
  void GetBrowserWindowCount(int* window_count);
  void GetNormalBrowserWindowCount(int* window_count);
  
  
  void GetBrowserWindow(int index, int* handle);
  void FindNormalBrowserWindow(int* handle);
  void GetLastActiveBrowserWindow(int* handle);
  void GetActiveWindow(int* handle);
  void ExecuteBrowserCommandAsync(int handle, int command, bool* success);
  void ExecuteBrowserCommand(int handle, int command,
                             IPC::Message* reply_message);
  void GetBrowserLocale(string16* locale);
  void IsWindowActive(int handle, bool* success, bool* is_active);
  void ActivateWindow(int handle);
  void IsWindowMaximized(int handle, bool* is_maximized, bool* success);
  void TerminateSession(int handle, bool* success);
  void WindowGetViewBounds(int handle, int view_id, bool screen_coordinates,
                           bool* success, gfx::Rect* bounds);
  void GetWindowBounds(int handle, gfx::Rect* bounds, bool* result);
  void SetWindowBounds(int handle, const gfx::Rect& bounds, bool* result);
  void SetWindowVisible(int handle, bool visible, bool* result);
  void WindowSimulateClick(const IPC::Message& message,
                           int handle,
                           const gfx::Point& click,
                           int flags);
  void WindowSimulateMouseMove(const IPC::Message& message,
                               int handle,
                               const gfx::Point& location);
  void WindowSimulateKeyPress(const IPC::Message& message,
                              int handle,
                              int key,
                              int flags);
  void GetTabCount(int handle, int* tab_count);
  void GetType(int handle, int* type_as_int);
  void GetTab(int win_handle, int tab_index, int* tab_handle);
  void GetTabProcessID(int handle, int* process_id);
  void GetTabTitle(int handle, int* title_string_size, std::wstring* title);
  void GetTabIndex(int handle, int* tabstrip_index);
  void GetTabURL(int handle, bool* success, GURL* url);
  void GetShelfVisibility(int handle, bool* visible);
  void IsFullscreen(int handle, bool* is_fullscreen);
  void GetFullscreenBubbleVisibility(int handle, bool* is_visible);
  void GetAutocompleteEditForBrowser(int browser_handle, bool* success,
                                     int* autocomplete_edit_handle);

  
  void GetAutocompleteEditText(int autocomplete_edit_handle,
                               bool* success, string16* text);

  
  void SetAutocompleteEditText(int autocomplete_edit_handle,
                               const string16& text,
                               bool* success);

  
  void AutocompleteEditIsQueryInProgress(int autocomplete_edit_handle,
                                         bool* success,
                                         bool* query_in_progress);

  
  void AutocompleteEditGetMatches(int autocomplete_edit_handle,
                                  bool* success,
                                  std::vector<AutocompleteMatchData>* matches);

  
  void WaitForAutocompleteEditFocus(int autocomplete_edit_handle,
                                    IPC::Message* reply_message);

  void ExecuteJavascript(int handle,
                         const std::wstring& frame_xpath,
                         const std::wstring& script,
                         IPC::Message* reply_message);

  void GetConstrainedWindowCount(int handle, int* count);

#if defined(TOOLKIT_VIEWS)
  void GetFocusedViewID(int handle, int* view_id);

  
  
  void WaitForFocusedViewIDToChange(int handle,
                                    int previous_view_id,
                                    IPC::Message* reply_message);

  
  
  void StartTrackingPopupMenus(int browser_handle, bool* success);

  
  void WaitForPopupMenuToOpen(IPC::Message* reply_message);
#endif  

  void HandleInspectElementRequest(int handle,
                                   int x,
                                   int y,
                                   IPC::Message* reply_message);

  void GetDownloadDirectory(int handle, FilePath* download_directory);

  
  void OpenNewBrowserWindowOfType(int type,
                                  bool show,
                                  IPC::Message* reply_message);

  
  void GetWindowForBrowser(int window_handle, bool* success, int* handle);
  void GetBrowserForWindow(int window_handle, bool* success,
                           int* browser_handle);

  void ShowInterstitialPage(int tab_handle,
                            const std::string& html_text,
                            IPC::Message* reply_message);
  void HideInterstitialPage(int tab_handle, bool* success);

  void WaitForTabToBeRestored(int tab_handle, IPC::Message* reply_message);

  
  void GetSecurityState(int handle, bool* success,
                        SecurityStyle* security_style, int* ssl_cert_status,
                        int* insecure_content_status);

  
  void GetPageType(int handle, bool* success, PageType* page_type);

  
  
  void GetMetricEventDuration(const std::string& event_name, int* duration_ms);

  
  
  
  
  void ActionOnSSLBlockingPage(int handle,
                               bool proceed,
                               IPC::Message* reply_message);

  
  void BringBrowserToFront(int browser_handle, bool* success);

  
  void IsMenuCommandEnabled(int browser_handle,
                            int message_num,
                            bool* menu_item_enabled);

  
  void PrintNow(int tab_handle, IPC::Message* reply_message);

  
  void SavePage(int tab_handle,
                const FilePath& file_name,
                const FilePath& dir_path,
                int type,
                bool* success);

  
  void HandleOpenFindInPageRequest(const IPC::Message& message,
                                   int handle);

  
  void GetFindWindowVisibility(int handle, bool* visible);

  
  void HandleFindWindowLocationRequest(int handle, int* x, int* y);

  
  void GetBookmarkBarVisibility(int handle, bool* visible, bool* animating);

  
  void GetBookmarksAsJSON(int handle, std::string* bookmarks_as_json,
                          bool *success);

  
  void WaitForBookmarkModelToLoad(int handle, IPC::Message* reply_message);

  
  void BookmarkModelHasLoaded(int handle, bool* loaded);

  
  
  void AddBookmarkGroup(int handle,
                        int64 parent_id, int index, std::wstring title,
                        bool* success);
  void AddBookmarkURL(int handle,
                      int64 parent_id, int index,
                      std::wstring title, const GURL& url,
                      bool* success);
  void ReparentBookmark(int handle,
                        int64 id, int64 new_parent_id, int index,
                        bool* success);
  void SetBookmarkTitle(int handle,
                        int64 id, std::wstring title,
                        bool* success);
  void SetBookmarkURL(int handle,
                      int64 id, const GURL& url,
                      bool* success);
  void RemoveBookmark(int handle,
                      int64 id,
                      bool* success);

  
  void GetInfoBarCount(int handle, size_t* count);

  
  
  
  void ClickInfoBarAccept(int handle,
                          size_t info_bar_index,
                          bool wait_for_navigation,
                          IPC::Message* reply_message);

  
  void GetLastNavigationTime(int handle, int64* last_navigation_time);

  
  
  void WaitForNavigation(int handle,
                         int64 last_navigation_time,
                         IPC::Message* reply_message);

  
  void SetIntPreference(int handle,
                        const std::string& name,
                        int value,
                        bool* success);

  
  void SetStringPreference(int handle,
                           const std::string& name,
                           const std::string& value,
                           bool* success);

  
  void GetBooleanPreference(int handle,
                            const std::string& name,
                            bool* success,
                            bool* value);

  
  void SetBooleanPreference(int handle,
                            const std::string& name,
                            bool value,
                            bool* success);

  void GetShowingAppModalDialog(bool* showing_dialog, int* dialog_button);
  void ClickAppModalDialogButton(int button, bool* success);

  void WaitForBrowserWindowCountToBecome(int target_count,
                                         IPC::Message* reply_message);

  void WaitForAppModalDialogToBeShown(IPC::Message* reply_message);

  void GoBackBlockUntilNavigationsComplete(int handle,
                                           int number_of_navigations,
                                           IPC::Message* reply_message);

  void GoForwardBlockUntilNavigationsComplete(int handle,
                                              int number_of_navigations,
                                              IPC::Message* reply_message);

  void SavePackageShouldPromptUser(bool should_prompt);

  void GetWindowTitle(int handle, string16* text);

  void SetShelfVisibility(int handle, bool visible);

  
  void GetBlockedPopupCount(int handle, int* count);

  
  
  void SendJSONRequest(int handle,
                       const std::string& json_request,
                       IPC::Message* reply_message);

  
  
  typedef void (TestingAutomationProvider::*JsonHandler)(DictionaryValue*,
                                                         IPC::Message*);

  
  
  typedef void (TestingAutomationProvider::*BrowserJsonHandler)(
      Browser* browser,
      DictionaryValue*,
      IPC::Message*);

    
  
  void SetWindowDimensions(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  
  ListValue* GetInfobarsInfo(TabContents* tc);

  
  
  void PerformActionOnInfobar(Browser* browser,
                              DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  
  void GetBrowserInfo(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  
  void GetNavigationInfo(Browser* browser,
                         DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  
  void GetDownloadsInfo(Browser* browser,
                        DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  void WaitForDownloadsToComplete(Browser* browser,
                                  DictionaryValue* args,
                                  IPC::Message* reply_message);

  
  
  void PerformActionOnDownload(Browser* browser,
                               DictionaryValue* args,
                               IPC::Message* reply_message);

  
  
  void GetHistoryInfo(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void AddHistoryItem(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void LoadSearchEngineInfo(Browser* browser,
                            DictionaryValue* args,
                            IPC::Message* reply_message);

  
  
  
  void GetSearchEngineInfo(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  void AddOrEditSearchEngine(Browser* browser,
                             DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  
  void PerformActionOnSearchEngine(Browser* browser,
                                   DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  void GetPrefsInfo(Browser* browser,
                    DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void SetPrefs(Browser* browser,
                DictionaryValue* args,
                IPC::Message* reply_message);

  
  
  
  
  
  void GetInitialLoadTimes(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void GetPluginsInfo(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void EnablePlugin(Browser* browser,
                    DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void DisablePlugin(Browser* browser,
                     DictionaryValue* args,
                     IPC::Message* reply_message);

  
  
  
  
  void GetOmniboxInfo(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void SetOmniboxText(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void OmniboxMovePopupSelection(Browser* browser,
                                 DictionaryValue* args,
                                 IPC::Message* reply_message);

  
  
  
  
  void OmniboxAcceptInput(Browser* browser,
                          DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void GetInstantInfo(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  void SaveTabContents(Browser* browser,
                       DictionaryValue* args,
                       IPC::Message* reply_message);

  
  
  void ImportSettings(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  
  
  
  void AddSavedPassword(Browser* browser,
                        DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  
  void RemoveSavedPassword(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void GetSavedPasswords(Browser* browser,
                         DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  
  void ClearBrowsingData(Browser* browser,
                         DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void GetBlockedPopupsInfo(Browser* browser,
                            DictionaryValue* args,
                            IPC::Message* reply_message);

  
  
  void UnblockAndLaunchBlockedPopup(Browser* browser,
                                    DictionaryValue* args,
                                    IPC::Message* reply_message);

  
  
  void GetThemeInfo(Browser* browser,
                    DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void GetExtensionsInfo(Browser* browser,
                         DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void UninstallExtensionById(Browser* browser,
                              DictionaryValue* args,
                              IPC::Message* reply_message);

  
  void FindInPage(Browser* browser,
                  DictionaryValue* args,
                  IPC::Message* reply_message);

  
  
  void GetTranslateInfo(Browser* browser,
                        DictionaryValue* args,
                        IPC::Message* reply_message);

  
  
  void SelectTranslateOption(Browser* browser,
                             DictionaryValue* args,
                             IPC::Message* reply_message);

  
  
  void GetAutofillProfile(Browser* browser,
                          DictionaryValue* args,
                          IPC::Message* reply_message);

  
  
  void FillAutofillProfile(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  void SignInToSync(Browser* browser,
                    DictionaryValue* args,
                    IPC::Message* reply_message);

  
  
  void GetSyncInfo(Browser* browser,
                   DictionaryValue* args,
                   IPC::Message* reply_message);

  
  
  void AwaitSyncCycleCompletion(Browser* browser,
                                DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  void EnableSyncForDatatypes(Browser* browser,
                              DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  void DisableSyncForDatatypes(Browser* browser,
                               DictionaryValue* args,
                               IPC::Message* reply_message);

  
  
  
  
  
  static std::vector<AutofillProfile> GetAutofillProfilesFromList(
      const ListValue& profiles, std::string* error_message);
  static std::vector<CreditCard> GetCreditCardsFromList(
      const ListValue& cards, std::string* error_message);

  
  
  
  static ListValue* GetListFromAutofillProfiles(
      const std::vector<AutofillProfile*>& autofill_profiles);
  static ListValue* GetListFromCreditCards(
      const std::vector<CreditCard*>& credit_cards);

  
  
  static std::map<AutofillFieldType, std::string>
      GetAutofillFieldToStringMap();
  static std::map<AutofillFieldType, std::string>
      GetCreditCardFieldToStringMap();

  
  
  void GetActiveNotifications(Browser* browser,
                              DictionaryValue* args,
                              IPC::Message* reply_message);

  
  
  void CloseNotification(Browser* browser,
                         DictionaryValue* args,
                         IPC::Message* reply_message);

  
  
  void WaitForNotificationCount(Browser* browser,
                                DictionaryValue* args,
                                IPC::Message* reply_message);

  
  
  void GetNTPInfo(Browser* browser,
                  DictionaryValue* args,
                  IPC::Message* reply_message);

  
  
  
  void MoveNTPMostVisitedThumbnail(Browser* browser,
                                   DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  void RemoveNTPMostVisitedThumbnail(Browser* browser,
                                     DictionaryValue* args,
                                     IPC::Message* reply_message);

  
  
  void UnpinNTPMostVisitedThumbnail(Browser* browser,
                                    DictionaryValue* args,
                                    IPC::Message* reply_message);

  
  
  
  void RestoreAllNTPMostVisitedThumbnails(Browser* browser,
                                          DictionaryValue* args,
                                          IPC::Message* reply_message);

  
  
  void KillRendererProcess(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  bool BuildWebKeyEventFromArgs(DictionaryValue* args,
                                std::string* error,
                                NativeWebKeyboardEvent* event);

  
  void GetNTPThumbnailMode(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  
  
  void SetNTPThumbnailMode(Browser* browser,
                           DictionaryValue* args,
                           IPC::Message* reply_message);

  
  void GetNTPMenuMode(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  void SetNTPMenuMode(Browser* browser,
                      DictionaryValue* args,
                      IPC::Message* reply_message);

  
  void LaunchApp(Browser* browser,
                 DictionaryValue* args,
                 IPC::Message* reply_message);

  
  void SetAppLaunchType(Browser* browser,
                        DictionaryValue* args,
                        IPC::Message* reply_message);

  
  void WaitForAllTabsToStopLoading(DictionaryValue* args,
                                   IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void GetIndicesFromTab(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  void NavigateToURL(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExecuteJavascriptJSON(
      DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  void GoForward(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  void GoBack(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void ReloadJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void GetTabURLJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void GetTabTitleJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  void CaptureEntirePageJSON(
      DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void GetCookiesJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  void DeleteCookieJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetCookieJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void GetTabIds(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void IsTabIdValid(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void CloseTabJSON(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  
  
  
  
  
  void WebkitMouseClick(DictionaryValue* args,
                        IPC::Message* message);

  
  
  
  
  
  
  
  
  void WebkitMouseMove(DictionaryValue* args,
                       IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  void WebkitMouseDrag(DictionaryValue* args,
                       IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  
  
  
  void SendWebkitKeyEvent(DictionaryValue* args,
                          IPC::Message* message);

  
  
  
  
  
  
  
  
  
  
  void SendOSLevelKeyEventToTab(DictionaryValue* args,
                                IPC::Message* message);

  
  void SendSuccessReply(IPC::Message* reply_message);

  
  
  
  
  
  
  void ActivateTabJSON(DictionaryValue* args, IPC::Message* message);

  
  
  void UpdateExtensionsNow(DictionaryValue* args, IPC::Message* reply_message);

  
  
  
  
  void GetChromeDriverAutomationVersion(DictionaryValue* args,
                                        IPC::Message* message);

#if defined(OS_CHROMEOS)
  void GetLoginInfo(DictionaryValue* args, IPC::Message* reply_message);

  void LoginAsGuest(DictionaryValue* args, IPC::Message* reply_message);

  void Login(DictionaryValue* args, IPC::Message* reply_message);

  void LockScreen(DictionaryValue* args, IPC::Message* reply_message);

  void UnlockScreen(DictionaryValue* args, IPC::Message* reply_message);

  void SignoutInScreenLocker(DictionaryValue* args,
                             IPC::Message* reply_message);

  void GetBatteryInfo(DictionaryValue* args, IPC::Message* reply_message);

  void GetNetworkInfo(DictionaryValue* args, IPC::Message* reply_message);

  void NetworkScan(DictionaryValue* args, IPC::Message* reply_message);

  void GetProxySettings(DictionaryValue* args, IPC::Message* reply_message);

  void SetProxySettings(DictionaryValue* args, IPC::Message* reply_message);

  void ConnectToWifiNetwork(DictionaryValue* args, IPC::Message* reply_message);

  void ConnectToHiddenWifiNetwork(DictionaryValue* args,
                                  IPC::Message* reply_message);

  void DisconnectFromWifiNetwork(DictionaryValue* args,
                                 IPC::Message* reply_message);

  void GetUpdateInfo(DictionaryValue* args, IPC::Message* reply_message);

  void UpdateCheck(DictionaryValue* args, IPC::Message* reply_message);

  void SetReleaseTrack(DictionaryValue* args, IPC::Message* reply_message);
#endif  

  void WaitForTabCountToBecome(int browser_handle,
                               int target_tab_count,
                               IPC::Message* reply_message);

  void WaitForInfoBarCount(int tab_handle,
                           size_t target_count,
                           IPC::Message* reply_message);

  
  void GetPageCurrentEncoding(int tab_handle, std::string* current_encoding);

  void ShutdownSessionService(int handle, bool* result);

  void SetContentSetting(int handle,
                         const std::string& host,
                         ContentSettingsType content_type,
                         ContentSetting setting,
                         bool* success);

  
  void LoadBlockedPlugins(int tab_handle, bool* success);

  
  void ResetToDefaultTheme();

  void WaitForProcessLauncherThreadToGoIdle(IPC::Message* reply_message);

  
  void GetParentBrowserOfTab(
      int tab_handle, int* browser_handle, bool* success);

  
  virtual void OnRedirectQueryComplete(
      HistoryService::Handle request_handle,
      GURL from_url,
      bool success,
      history::RedirectList* redirects);

  void OnRemoveProvider();  

#if defined(TOOLKIT_VIEWS)
  
  
  bool popup_menu_opened_;

  
  PopupMenuWaiter* popup_menu_waiter_;
#endif  

  
  scoped_ptr<ProfileSyncServiceHarness> sync_waiter_;

  
  
  
  
  HistoryService::Handle redirect_query_;

  NotificationRegistrar registrar_;

  
  scoped_refptr<ImporterList> importer_list_;

  
  ImportSettingsData import_settings_data_;

  DISALLOW_COPY_AND_ASSIGN(TestingAutomationProvider);
};

#endif  
