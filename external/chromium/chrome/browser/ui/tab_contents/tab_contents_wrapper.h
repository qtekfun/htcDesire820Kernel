// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_WRAPPER_H_
#define CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_WRAPPER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/printing/print_view_manager.h"
#include "content/browser/tab_contents/tab_contents.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "content/common/notification_registrar.h"

namespace prerender {
class PrerenderObserver;
}

namespace printing {
class PrintPreviewMessageHandler;
}

class AutocompleteHistoryManager;
class AutofillManager;
class AutomationTabHelper;
class DownloadTabHelper;
class Extension;
class ExtensionTabHelper;
class ExtensionWebNavigationTabObserver;
class FileSelectObserver;
class FindTabHelper;
class NavigationController;
class PasswordManager;
class PasswordManagerDelegate;
class SearchEngineTabHelper;
class TabContentsWrapperDelegate;
class TranslateTabHelper;

class TabContentsWrapper : public NotificationObserver,
                           public TabContentsObserver {
 public:
  
  
  explicit TabContentsWrapper(TabContents* contents);
  ~TabContentsWrapper();

  
  
  static PropertyAccessor<TabContentsWrapper*>* property_accessor();

  static void RegisterUserPrefs(PrefService* prefs);

  
  static string16 GetDefaultTitle();

  
  string16 GetStatusText() const;

  
  
  TabContentsWrapper* Clone();

  
  
  
  
  
  
  static TabContentsWrapper* GetCurrentWrapperForContents(
      TabContents* contents);

  TabContentsWrapperDelegate* delegate() const { return delegate_; }
  void set_delegate(TabContentsWrapperDelegate* d) { delegate_ = d; }

  TabContents* tab_contents() const { return tab_contents_.get(); }
  NavigationController& controller() const {
    return tab_contents()->controller();
  }
  TabContentsView* view() const { return tab_contents()->view(); }
  RenderViewHost* render_view_host() const {
    return tab_contents()->render_view_host();
  }
  Profile* profile() const { return tab_contents()->profile(); }
  bool is_starred() const { return is_starred_; }

  

  AutocompleteHistoryManager* autocomplete_history_manager() {
    return autocomplete_history_manager_.get();
  }

  AutofillManager* autofill_manager() { return autofill_manager_.get(); }

  
  AutomationTabHelper* automation_tab_helper() {
    return automation_tab_helper_.get();
  }

  DownloadTabHelper* download_tab_helper() {
    return download_tab_helper_.get();
  }

  ExtensionTabHelper* extension_tab_helper() {
    return extension_tab_helper_.get();
  }

  FindTabHelper* find_tab_helper() { return find_tab_helper_.get(); }

  PasswordManager* password_manager() { return password_manager_.get(); }

  printing::PrintViewManager* print_view_manager() {
    return print_view_manager_.get();
  }

  SearchEngineTabHelper* search_engine_tab_helper() {
    return search_engine_tab_helper_.get();
  }

  TranslateTabHelper* translate_tab_helper() {
    return translate_tab_helper_.get();
  }

  

  
  virtual void DidNavigateMainFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

 private:
  

  
  void OnPageContents(const GURL& url,
                      int32 page_id,
                      const string16& contents);
  void OnJSOutOfMemory();
  void OnRegisterProtocolHandler(const std::string& protocol,
                                 const GURL& url,
                                 const string16& title);
  void OnMsgThumbnail(const GURL& url,
                      const ThumbnailScore& score,
                      const SkBitmap& bitmap);

  
  
  void UpdateStarredStateForCurrentURL();

  

  
  TabContentsWrapperDelegate* delegate_;

  
  NotificationRegistrar registrar_;

  

  
  bool is_starred_;

  
  
  

  scoped_ptr<AutocompleteHistoryManager> autocomplete_history_manager_;
  scoped_ptr<AutofillManager> autofill_manager_;
  scoped_ptr<AutomationTabHelper> automation_tab_helper_;
  scoped_ptr<DownloadTabHelper> download_tab_helper_;
  scoped_ptr<ExtensionTabHelper> extension_tab_helper_;
  scoped_ptr<FindTabHelper> find_tab_helper_;

  
  
  scoped_ptr<PasswordManagerDelegate> password_manager_delegate_;
  scoped_ptr<PasswordManager> password_manager_;

  
  scoped_ptr<printing::PrintViewManager> print_view_manager_;

  scoped_ptr<SearchEngineTabHelper> search_engine_tab_helper_;
  scoped_ptr<TranslateTabHelper> translate_tab_helper_;

  
  
  

  scoped_ptr<FileSelectObserver> file_select_observer_;
  scoped_ptr<prerender::PrerenderObserver> prerender_observer_;
  scoped_ptr<printing::PrintPreviewMessageHandler> print_preview_;
  scoped_ptr<ExtensionWebNavigationTabObserver> webnavigation_observer_;

  

  
  
  
  scoped_ptr<TabContents> tab_contents_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsWrapper);
};

#endif  
