// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_H_
#define CHROME_BROWSER_TAB_CONTENTS_TAB_CONTENTS_H_
#pragma once

#ifdef ANDROID
#include "android/autofill/profile_android.h"
#include "base/scoped_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/autofill/autofill_host.h"

class RenderViewHost;
class URLRequestContextGetter;

class TabContents {
public:
  TabContents()
    : profile_(ProfileImplAndroid::CreateProfile(FilePath()))
    , autofill_host_(NULL)
  {
  }

  Profile* profile() { return profile_.get(); }
  void SetProfileRequestContext(URLRequestContextGetter* context) { static_cast<ProfileImplAndroid*>(profile_.get())->SetRequestContext(context); }
  AutoFillHost* autofill_host() { return autofill_host_; }
  void SetAutoFillHost(AutoFillHost* autofill_host) { autofill_host_ = autofill_host; }

private:
  scoped_ptr<Profile> profile_;
  AutoFillHost* autofill_host_;
};

#else

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/dom_ui/web_ui_factory.h"
#include "chrome/browser/download/save_package.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/fav_icon_helper.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/renderer_host/render_view_host_delegate.h"
#include "chrome/browser/tab_contents/constrained_window.h"
#include "chrome/browser/tab_contents/language_state.h"
#include "chrome/browser/tab_contents/navigation_controller.h"
#include "chrome/browser/tab_contents/navigation_entry.h"
#include "chrome/browser/tab_contents/page_navigator.h"
#include "chrome/browser/tab_contents/render_view_host_manager.h"
#include "chrome/browser/tab_contents/tab_specific_content_settings.h"
#include "chrome/browser/ui/app_modal_dialogs/js_modal_dialog.h"
#include "chrome/common/notification_registrar.h"
#include "chrome/common/property_bag.h"
#include "chrome/common/renderer_preferences.h"
#include "chrome/common/translate_errors.h"
#include "chrome/common/web_apps.h"
#include "net/base/load_states.h"
#include "ui/gfx/native_widget_types.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

namespace gfx {
class Rect;
}

namespace history {
class HistoryAddPageArgs;
}

namespace printing {
class PrintPreviewMessageHandler;
class PrintViewManager;
}

class AutocompleteHistoryManager;
class AutoFillManager;
class BlockedContentContainer;
class WebUI;
class DesktopNotificationHandler;
class DownloadItem;
class Extension;
class FileSelectHelper;
class InfoBarDelegate;
class LoadNotificationDetails;
class OmniboxSearchHint;
class PluginObserver;
class Profile;
class PrerenderManager;
class PrerenderPLTRecorder;
class RenderViewHost;
class SessionStorageNamespace;
class SiteInstance;
class SkBitmap;
class TabContents;
class TabContentsDelegate;
class TabContentsObserver;
class TabContentsSSLHelper;
class TabContentsView;
class URLPattern;
struct RendererPreferences;
struct ThumbnailScore;
struct ViewHostMsg_DomMessage_Params;
struct ViewHostMsg_FrameNavigate_Params;
struct ViewHostMsg_RunFileChooser_Params;
struct WebPreferences;

class TabContents : public PageNavigator,
                    public NotificationObserver,
                    public RenderViewHostDelegate,
                    public RenderViewHostManager::Delegate,
                    public JavaScriptAppModalDialogDelegate,
                    public ImageLoadingTracker::Observer,
                    public TabSpecificContentSettings::Delegate {
 public:
  
  
  enum InvalidateTypes {
    INVALIDATE_URL             = 1 << 0,  
    INVALIDATE_TAB             = 1 << 1,  
                                          
    INVALIDATE_LOAD            = 1 << 2,  
    INVALIDATE_PAGE_ACTIONS    = 1 << 3,  
    INVALIDATE_BOOKMARK_BAR    = 1 << 4,  
                                          
    INVALIDATE_TITLE           = 1 << 5,  
  };

  
  
  
  
  
  
  
  TabContents(Profile* profile,
              SiteInstance* site_instance,
              int routing_id,
              const TabContents* base_tab_contents,
              SessionStorageNamespace* session_storage_namespace);
  virtual ~TabContents();

  static void RegisterUserPrefs(PrefService* prefs);

  

  
  
  
  const PropertyBag* property_bag() const { return &property_bag_; }
  PropertyBag* property_bag() { return &property_bag_; }

  TabContentsDelegate* delegate() const { return delegate_; }
  void set_delegate(TabContentsDelegate* d) { delegate_ = d; }

  
  NavigationController& controller() { return controller_; }
  const NavigationController& controller() const { return controller_; }

  
  
  Profile* profile() const { return controller_.profile(); }

  
  bool HostsExtension() const;

  
  TabContentsSSLHelper* GetSSLHelper();

  
  SavePackage* save_package() const { return save_package_.get(); }

  
  
  RenderProcessHost* GetRenderProcessHost() const;
  RenderViewHost* render_view_host() const {
    return render_manager_.current_host();
  }

  WebUI* web_ui() const {
    return render_manager_.web_ui() ? render_manager_.web_ui()
        : render_manager_.pending_web_ui();
  }

  
  
  RenderWidgetHostView* GetRenderWidgetHostView() const {
    return render_manager_.GetRenderWidgetHostView();
  }

  
  TabContentsView* view() const {
    return view_.get();
  }

  
  FavIconHelper& fav_icon_helper() {
    return fav_icon_helper_;
  }

  

  
  
  
  
  
  
  
  void SetExtensionApp(const Extension* extension);

  
  
  
  void SetExtensionAppById(const std::string& extension_app_id);

  const Extension* extension_app() const { return extension_app_; }
  bool is_app() const { return extension_app_ != NULL; }

  
  
  
  
  
  SkBitmap* GetExtensionAppIcon();

  

  
  
  
  virtual const GURL& GetURL() const;
  virtual const string16& GetTitle() const;

  
  static string16 GetDefaultTitle();

  
  
  
  
  int32 GetMaxPageID();

  
  void UpdateMaxPageID(int32 page_id);

  
  
  
  virtual SiteInstance* GetSiteInstance() const;

  
  
  
  
  virtual bool ShouldDisplayURL();

  
  
  
  SkBitmap GetFavIcon() const;

  
  bool FavIconIsValid() const;

  
  
  virtual bool ShouldDisplayFavIcon();

  
  virtual string16 GetStatusText() const;

  
  
  
  
  void AddObserver(TabContentsObserver* observer);
  void RemoveObserver(TabContentsObserver* observer);

  
  bool is_loading() const { return is_loading_; }

  
  
  
  bool waiting_for_response() const { return waiting_for_response_; }

  const std::string& encoding() const { return encoding_; }
  void set_encoding(const std::string& encoding);
  void reset_encoding() {
    encoding_.clear();
  }

  const WebApplicationInfo& web_app_info() const {
    return web_app_info_;
  }

  const SkBitmap& app_icon() const { return app_icon_; }

  
  
  void SetAppIcon(const SkBitmap& app_icon);

  bool displayed_insecure_content() const {
    return displayed_insecure_content_;
  }

  

  
  
  bool capturing_contents() const { return capturing_contents_; }
  void set_capturing_contents(bool cap) { capturing_contents_ = cap; }

  
  
  bool is_crashed() const {
    return (crashed_status_ == base::TERMINATION_STATUS_PROCESS_CRASHED ||
            crashed_status_ == base::TERMINATION_STATUS_ABNORMAL_TERMINATION ||
            crashed_status_ == base::TERMINATION_STATUS_PROCESS_WAS_KILLED);
  }
  base::TerminationStatus crashed_status() const { return crashed_status_; }
  int crashed_error_code() const { return crashed_error_code_; }
  void SetIsCrashed(base::TerminationStatus status, int error_code);

  
  void PageActionStateChanged();

  
  
  
  bool is_being_destroyed() const { return is_being_destroyed_; }

  
  
  void NotifyNavigationStateChanged(unsigned changed_flags);

  
  
  virtual void DidBecomeSelected();
  base::TimeTicks last_selected_time() const {
    return last_selected_time_;
  }

  
  
  virtual void WasHidden();

  
  
  void Activate();

  
  void Deactivate();

  
  virtual void ShowContents();
  virtual void HideContents();

  
  
  
  
  bool NeedToFireBeforeUnload();

#ifdef UNIT_TEST
  
  RenderViewHostManager* render_manager() { return &render_manager_; }
#endif

  
  
  
  void SwapInRenderViewHost(RenderViewHost* rvh);

  

  
  virtual void OpenURL(const GURL& url, const GURL& referrer,
                       WindowOpenDisposition disposition,
                       PageTransition::Type transition);

  
  
  
  
  
  
  
  
  
  
  
  virtual bool NavigateToPendingEntry(
      NavigationController::ReloadType reload_type);

  
  virtual void Stop();

  
  virtual void DisassociateFromPopupCount();

  
  
  virtual TabContents* Clone();

  
  void ShowPageInfo(const GURL& url,
                    const NavigationEntry::SSLStatus& ssl,
                    bool show_history);

  
  void SaveFavicon();

  

  
  
  
  
  ConstrainedWindow* CreateConstrainedDialog(
      ConstrainedWindowDelegate* delegate);

  
  void AddNewContents(TabContents* new_contents,
                      WindowOpenDisposition disposition,
                      const gfx::Rect& initial_pos,
                      bool user_gesture);

  
  
  bool ExecuteCode(int request_id, const std::string& extension_id,
                   bool is_js_code, const std::string& code_string,
                   bool all_frames);

  
  virtual void PopupNotificationVisibilityChanged(bool visible);

  
  size_t constrained_window_count() { return child_windows_.size(); }

  typedef std::deque<ConstrainedWindow*> ConstrainedWindowList;

  
  ConstrainedWindowList::iterator constrained_window_begin()
  { return child_windows_.begin(); }

  
  ConstrainedWindowList::iterator constrained_window_end()
  { return child_windows_.end(); }

  
  
  

  
  gfx::NativeView GetContentNativeView() const;

  
  
  gfx::NativeView GetNativeView() const;

  
  void GetContainerBounds(gfx::Rect *out) const;

  
  void Focus();

  
  
  
  void FocusThroughTabTraversal(bool reverse);

  
  

  
  
  
  virtual bool FocusLocationBarByDefault();

  
  virtual void SetFocusToLocationBar(bool select_all);

  
  virtual void CreateViewAndSetSizeForRVH(RenderViewHost* rvh);

  

  
  virtual void AddInfoBar(InfoBarDelegate* delegate);

  
  void RemoveInfoBar(InfoBarDelegate* delegate);

  
  void ReplaceInfoBar(InfoBarDelegate* old_delegate,
                      InfoBarDelegate* new_delegate);

  
  size_t infobar_count() const { return infobar_delegates_.size(); }
  
  InfoBarDelegate* GetInfoBarDelegateAt(size_t index) {
    return infobar_delegates_[index];
  }

  

  
  virtual bool ShouldShowBookmarkBar();

  
  
  bool CanDownload(int request_id);

  
  void OnStartDownload(DownloadItem* download);

  
  void ToolbarSizeChanged(bool is_animating);

  
  void WillClose(ConstrainedWindow* window);

  
  void WillCloseBlockedContentContainer(BlockedContentContainer* container);

  
  void DidMoveOrResize(ConstrainedWindow* window);

  

  
  bool showing_interstitial_page() const {
    return render_manager_.interstitial_page() != NULL;
  }

  
  
  
  
  void set_interstitial_page(InterstitialPage* interstitial_page) {
    render_manager_.set_interstitial_page(interstitial_page);
  }

  
  void remove_interstitial_page() {
    render_manager_.remove_interstitial_page();
  }

  
  
  InterstitialPage* interstitial_page() const {
    return render_manager_.interstitial_page();
  }

  

  
  
  void set_suppress_javascript_messages(bool suppress_javascript_messages) {
    suppress_javascript_messages_ = suppress_javascript_messages;
  }

  
  void OnSavePage();

  
  
  
  bool SavePage(const FilePath& main_file, const FilePath& dir_path,
                SavePackage::SavePackageType save_type);

  
  void SetSavePackage(SavePackage* save_package);

  
  
  void EmailPageLocation();

  
  
  
  
  
  void PrintPreview();

  
  
  
  bool PrintNow();

  
  void PrintingDone(int document_cookie, bool success);

  
  bool IsActiveEntry(int32 page_id);

  const std::string& contents_mime_type() const {
    return contents_mime_type_;
  }

  
  bool notify_disconnection() const { return notify_disconnection_; }

  
  
  
  
  
  void SetOverrideEncoding(const std::string& encoding);

  
  
  void ResetOverrideEncoding();

  void WindowMoveOrResizeStarted();

  
  
  
  
  void SetAllContentsBlocked(bool value);

  BlockedContentContainer* blocked_content_container() const {
    return blocked_contents_;
  }

  RendererPreferences* GetMutableRendererPrefs() {
    return &renderer_preferences_;
  }

  void set_opener_web_ui_type(WebUITypeID opener_web_ui_type) {
    opener_web_ui_type_ = opener_web_ui_type;
  }

  
  
  void LogNewTabTime(const std::string& event_name);

  
  
  void set_new_tab_start_time(const base::TimeTicks& time) {
    new_tab_start_time_ = time;
  }

  
  
  void OnCloseStarted();

  LanguageState& language_state() {
    return language_state_;
  }

  
  bool ShouldAcceptDragAndDrop() const;

  
  
  void SystemDragEnded();

  
  
  
  
  void set_closed_by_user_gesture(bool value) {
    closed_by_user_gesture_ = value;
  }
  bool closed_by_user_gesture() const { return closed_by_user_gesture_; }

  
  virtual void OnMessageBoxClosed(IPC::Message* reply_msg,
                                  bool success,
                                  const std::wstring& prompt);
  virtual void SetSuppressMessageBoxes(bool suppress_message_boxes);
  virtual gfx::NativeWindow GetMessageBoxRootWindow();
  virtual TabContents* AsTabContents();
  virtual ExtensionHost* AsExtensionHost();

  
  
  virtual RenderViewHostDelegate::BookmarkDrag* GetBookmarkDragDelegate();

  
  
  
  virtual void SetBookmarkDragDelegate(
      RenderViewHostDelegate::BookmarkDrag* bookmark_drag);

  
  
  TabSpecificContentSettings* GetTabSpecificContentSettings() const;

  
  
  void UpdateHistoryForNavigation(
      scoped_refptr<history::HistoryAddPageArgs> add_page_args);

  
  
  void UpdateHistoryPageTitle(const NavigationEntry& entry);

  
  double GetZoomLevel() const;

  
  int GetZoomPercent(bool* enable_increment, bool* enable_decrement);

  
  
  void FadeForInstant(bool animate);

  
  void CancelInstantFade();

  
  void ViewSource();

  
  int minimum_zoom_percent() const { return minimum_zoom_percent_; }
  int maximum_zoom_percent() const { return maximum_zoom_percent_; }

  int content_restrictions() const { return content_restrictions_; }

  AutocompleteHistoryManager* autocomplete_history_manager() {
    return autocomplete_history_manager_.get();
  }
  AutoFillManager* autofill_manager() { return autofill_manager_.get(); }

 protected:
  
  virtual bool OnMessageReceived(const IPC::Message& message);

 private:
  friend class NavigationController;
  
  
  friend class TestingAutomationProvider;

  FRIEND_TEST_ALL_PREFIXES(TabContentsTest, NoJSMessageOnInterstitials);
  FRIEND_TEST_ALL_PREFIXES(TabContentsTest, UpdateTitle);
  FRIEND_TEST_ALL_PREFIXES(TabContentsTest, CrossSiteCantPreemptAfterUnload);
  FRIEND_TEST_ALL_PREFIXES(FormStructureBrowserTest, HTMLFiles);
  FRIEND_TEST_ALL_PREFIXES(NavigationControllerTest, HistoryNavigate);
  FRIEND_TEST_ALL_PREFIXES(RenderViewHostManagerTest, PageDoesBackAndReload);

  
  friend class TabContentsView;
#if defined(OS_WIN)
  friend class TabContentsViewWin;
#elif defined(OS_MACOSX)
  friend class TabContentsViewMac;
#elif defined(TOOLKIT_USES_GTK)
  friend class TabContentsViewGtk;
#endif

  
  friend class InterstitialPage;

  
  friend class TestTabContents;

  
  friend class ExternalTabContainer;

  
  friend class PrerenderManager;

  
  void OnDidStartProvisionalLoadForFrame(int64 frame_id,
                                         bool main_frame,
                                         const GURL& url);
  void OnDidRedirectProvisionalLoad(int32 page_id,
                                    const GURL& source_url,
                                    const GURL& target_url);
  void OnDidFailProvisionalLoadWithError(int64 frame_id,
                                         bool main_frame,
                                         int error_code,
                                         const GURL& url,
                                         bool showing_repost_interstitial);
  void OnDidLoadResourceFromMemoryCache(const GURL& url,
                                        const std::string& frame_origin,
                                        const std::string& main_frame_origin,
                                        const std::string& security_info);
  void OnDidDisplayInsecureContent();
  void OnDidRunInsecureContent(const std::string& security_origin,
                               const GURL& target_url);
  void OnDocumentLoadedInFrame(int64 frame_id);
  void OnDidFinishLoad(int64 frame_id);
  void OnUpdateContentRestrictions(int restrictions);
  void OnPDFHasUnsupportedFeature();

  void OnGoToEntryAtOffset(int offset);
  void OnDidGetApplicationInfo(int32 page_id, const WebApplicationInfo& info);
  void OnInstallApplication(const WebApplicationInfo& info);
  void OnPageContents(const GURL& url,
                      int32 page_id,
                      const string16& contents,
                      const std::string& language,
                      bool page_translatable);
  void OnPageTranslated(int32 page_id,
                        const std::string& original_lang,
                        const std::string& translated_lang,
                        TranslateErrors::Type error_type);
  void OnSetSuggestions(int32 page_id,
                        const std::vector<std::string>& suggestions);
  void OnInstantSupportDetermined(int32 page_id, bool result);
  void OnRunFileChooser(const ViewHostMsg_RunFileChooser_Params& params);

  
  
  
  void SetIsLoading(bool is_loading,
                    LoadNotificationDetails* details);

  
  void AddPopup(TabContents* new_contents,
                const gfx::Rect& initial_pos);

  
  
  
  void SetNotWaitingForResponse() { waiting_for_response_ = false; }

  ConstrainedWindowList child_windows_;

  
  
  
  void ExpireInfoBars(
      const NavigationController::LoadCommittedDetails& details);

  
  
  WebUI* GetWebUIForCurrentState();

  
  
  

  
  
  
  
  void DidNavigateMainFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);
  void DidNavigateAnyFramePostCommit(
      RenderViewHost* render_view_host,
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);

  
  void CloseConstrainedWindows();

  
  
  virtual void UpdateAlternateErrorPageURL();

  
  void UpdateWebPreferences();

  
  void UpdateZoomLevel();

  
  
  
  void UpdateMaxPageIDIfNecessary(SiteInstance* site_instance,
                                  RenderViewHost* rvh);

  
  
  scoped_refptr<history::HistoryAddPageArgs> CreateHistoryAddPageArgs(
      const GURL& virtual_url,
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params);

  
  
  
  
  
  
  
  
  bool UpdateTitleForEntry(NavigationEntry* entry, const std::wstring& title);

  
  
  
  bool NavigateToEntry(const NavigationEntry& entry,
                       NavigationController::ReloadType reload_type);

  

  
  void NotifySwapped();
  void NotifyConnected();
  void NotifyDisconnected();

  
  void GenerateKeywordIfNecessary(
      const ViewHostMsg_FrameNavigate_Params& params);

  
  virtual void OnContentSettingsAccessed(bool content_was_blocked);

  

  
  virtual RenderViewHostDelegate::View* GetViewDelegate();
  virtual RenderViewHostDelegate::RendererManagement*
      GetRendererManagementDelegate();
  virtual RenderViewHostDelegate::ContentSettings* GetContentSettingsDelegate();
  virtual RenderViewHostDelegate::SSL* GetSSLDelegate();
  virtual AutomationResourceRoutingDelegate*
      GetAutomationResourceRoutingDelegate();
  virtual TabContents* GetAsTabContents();
  virtual ViewType::Type GetRenderViewType() const;
  virtual int GetBrowserWindowID() const;
  virtual void RenderViewCreated(RenderViewHost* render_view_host);
  virtual void RenderViewReady(RenderViewHost* render_view_host);
  virtual void RenderViewGone(RenderViewHost* render_view_host,
                              base::TerminationStatus status,
                              int error_code);
  virtual void RenderViewDeleted(RenderViewHost* render_view_host);
  virtual void DidNavigate(RenderViewHost* render_view_host,
                           const ViewHostMsg_FrameNavigate_Params& params);
  virtual void UpdateState(RenderViewHost* render_view_host,
                           int32 page_id,
                           const std::string& state);
  virtual void UpdateTitle(RenderViewHost* render_view_host,
                           int32 page_id,
                           const std::wstring& title);
  virtual void UpdateEncoding(RenderViewHost* render_view_host,
                              const std::string& encoding);
  virtual void UpdateTargetURL(int32 page_id, const GURL& url);
  virtual void UpdateThumbnail(const GURL& url,
                               const SkBitmap& bitmap,
                               const ThumbnailScore& score);
  virtual void UpdateInspectorSetting(const std::string& key,
                                      const std::string& value);
  virtual void ClearInspectorSettings();
  virtual void Close(RenderViewHost* render_view_host);
  virtual void RequestMove(const gfx::Rect& new_bounds);
  virtual void DidStartLoading();
  virtual void DidStopLoading();
  virtual void DidChangeLoadProgress(double progress);
  virtual void DocumentOnLoadCompletedInMainFrame(
      RenderViewHost* render_view_host,
      int32 page_id);
  virtual void RequestOpenURL(const GURL& url, const GURL& referrer,
                              WindowOpenDisposition disposition);
  virtual void DomOperationResponse(const std::string& json_string,
                                    int automation_id);
  virtual void ProcessWebUIMessage(const ViewHostMsg_DomMessage_Params& params);
  virtual void ProcessExternalHostMessage(const std::string& message,
                                          const std::string& origin,
                                          const std::string& target);
  virtual void RunJavaScriptMessage(const std::wstring& message,
                                    const std::wstring& default_prompt,
                                    const GURL& frame_url,
                                    const int flags,
                                    IPC::Message* reply_msg,
                                    bool* did_suppress_message);
  virtual void RunBeforeUnloadConfirm(const std::wstring& message,
                                      IPC::Message* reply_msg);
  virtual void ShowModalHTMLDialog(const GURL& url, int width, int height,
                                   const std::string& json_arguments,
                                   IPC::Message* reply_msg);
  virtual void PageHasOSDD(RenderViewHost* render_view_host,
                           int32 page_id,
                           const GURL& url,
                           const ViewHostMsg_PageHasOSDD_Type& provider_type);
  virtual GURL GetAlternateErrorPageURL() const;
  virtual RendererPreferences GetRendererPrefs(Profile* profile) const;
  virtual WebPreferences GetWebkitPrefs();
  virtual void OnUserGesture();
  virtual void OnIgnoredUIEvent();
  virtual void OnJSOutOfMemory();
  virtual void OnCrossSiteResponse(int new_render_process_host_id,
                                   int new_request_id);
  virtual void RendererUnresponsive(RenderViewHost* render_view_host,
                                    bool is_during_unload);
  virtual void RendererResponsive(RenderViewHost* render_view_host);
  virtual void LoadStateChanged(const GURL& url, net::LoadState load_state,
                                uint64 upload_position, uint64 upload_size);
  virtual bool IsExternalTabContainer() const;
  virtual void DidInsertCSS();
  virtual void FocusedNodeChanged(bool is_editable_node);
  virtual void UpdateZoomLimits(int minimum_percent,
                                int maximum_percent,
                                bool remember);
  virtual void WorkerCrashed();

  

  
  void BlockTabContent(bool blocked);

  virtual void BeforeUnloadFiredFromRenderManager(
      bool proceed,
      bool* proceed_to_fire_unload);
  virtual void DidStartLoadingFromRenderManager(
      RenderViewHost* render_view_host);
  virtual void RenderViewGoneFromRenderManager(
      RenderViewHost* render_view_host);
  virtual void UpdateRenderViewSizeForRenderManager();
  virtual void NotifySwappedFromRenderManager();
  virtual NavigationController& GetControllerForRenderManager();
  virtual WebUI* CreateWebUIForRenderManager(const GURL& url);
  virtual NavigationEntry* GetLastCommittedNavigationEntryForRenderManager();

  
  
  
  
  
  
  
  
  virtual bool CreateRenderViewForRenderManager(
      RenderViewHost* render_view_host);

  

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  

  
  const Extension* GetExtensionContaining(const GURL& url);

  
  
  void UpdateExtensionAppIcon(const Extension* extension);

  
  virtual void OnImageLoaded(SkBitmap* image, ExtensionResource resource,
                             int index);

  
  
  
  bool MaybeUsePreloadedPage(const GURL& url);

  

  
  TabContentsDelegate* delegate_;

  
  NavigationController controller_;

  
  scoped_ptr<TabContentsView> view_;

  

  
  RenderViewHostManager render_manager_;

  
  PropertyBag property_bag_;

  
  NotificationRegistrar registrar_;

  
  PrefChangeRegistrar pref_change_registrar_;

  
  scoped_ptr<printing::PrintViewManager> printing_;

  
  scoped_ptr<printing::PrintPreviewMessageHandler> print_preview_;

  
  scoped_refptr<SavePackage> save_package_;

  
  scoped_ptr<AutocompleteHistoryManager> autocomplete_history_manager_;

  
  scoped_ptr<AutoFillManager> autofill_manager_;

  
  scoped_ptr<PluginObserver> plugin_observer_;

  
  scoped_ptr<PrerenderPLTRecorder> prerender_plt_recorder_;

  
  scoped_ptr<TabContentsSSLHelper> ssl_helper_;

  
  scoped_ptr<FileSelectHelper> file_select_helper_;

  
  BookmarkDrag* bookmark_drag_;

  
  FavIconHelper fav_icon_helper_;

  
  WebApplicationInfo web_app_info_;

  
  SkBitmap app_icon_;

  
  scoped_ptr<TabSpecificContentSettings> content_settings_delegate_;

  
  scoped_ptr<DesktopNotificationHandler> desktop_notification_handler_;


  

  
  bool is_loading_;

  
  base::TerminationStatus crashed_status_;
  int crashed_error_code_;

  
  bool waiting_for_response_;

  
  
  
  
  int32 max_page_id_;

  
  base::TimeTicks current_load_start_;

  
  net::LoadState load_state_;
  string16 load_state_host_;
  
  
  uint64 upload_size_;
  uint64 upload_position_;

  

  
  
  
  
  bool received_page_title_;

  
  
  std::string contents_mime_type_;

  
  std::string encoding_;

  
  BlockedContentContainer* blocked_contents_;

  
  bool all_contents_blocked_;

  
  bool dont_notify_render_view_;

  
  bool displayed_insecure_content_;

  

  
  std::vector<InfoBarDelegate*> infobar_delegates_;

  

  
  
  const Extension* extension_app_;

  
  SkBitmap extension_app_icon_;

  
  scoped_ptr<ImageLoadingTracker> extension_app_image_loader_;

  

  
  bool capturing_contents_;

  
  bool is_being_destroyed_;

  
  
  
  
  bool notify_disconnection_;

  
  typedef std::map<FaviconService::Handle, int32> HistoryRequestMap;
  HistoryRequestMap history_requests_;

#if defined(OS_WIN)
  
  
  
  base::win::ScopedHandle message_box_active_;
#endif

  
  base::TimeTicks last_javascript_message_dismissal_;

  
  
  bool suppress_javascript_messages_;

  
  
  
  bool is_showing_before_unload_dialog_;

  
  
  scoped_ptr<OmniboxSearchHint> omnibox_search_hint_;

  
  RendererPreferences renderer_preferences_;

  
  
  WebUITypeID opener_web_ui_type_;

  
  base::TimeTicks new_tab_start_time_;

  
  base::TimeTicks tab_close_start_time_;

  
  base::TimeTicks last_selected_time_;

  
  LanguageState language_state_;

  
  bool closed_by_user_gesture_;

  
  int minimum_zoom_percent_;
  int maximum_zoom_percent_;
  
  
  
  bool temporary_zoom_settings_;

  
  ObserverList<TabContentsObserver> observers_;

  
  
  int content_restrictions_;

  DISALLOW_COPY_AND_ASSIGN(TabContents);
};

#endif 

#endif  
