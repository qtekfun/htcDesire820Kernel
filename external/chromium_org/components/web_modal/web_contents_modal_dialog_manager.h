// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_MANAGER_H_
#define COMPONENTS_WEB_MODAL_WEB_CONTENTS_MODAL_DIALOG_MANAGER_H_

#include <deque>

#include "base/memory/scoped_ptr.h"
#include "components/web_modal/native_web_contents_modal_dialog_manager.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ui/gfx/native_widget_types.h"

namespace web_modal {

class WebContentsModalDialogManagerDelegate;

class WebContentsModalDialogManager
    : public NativeWebContentsModalDialogManagerDelegate,
      public content::WebContentsObserver,
      public content::WebContentsUserData<WebContentsModalDialogManager> {
 public:
  virtual ~WebContentsModalDialogManager();

  WebContentsModalDialogManagerDelegate* delegate() const { return delegate_; }
  void SetDelegate(WebContentsModalDialogManagerDelegate* d);

  static NativeWebContentsModalDialogManager* CreateNativeManager(
      NativeWebContentsModalDialogManagerDelegate* native_delegate);

  
  
  void ShowDialog(NativeWebContentsModalDialog dialog);

  
  bool IsDialogActive() const;

  
  
  void FocusTopmostDialog();

  
  void SetCloseOnInterstitialPage(NativeWebContentsModalDialog dialog,
                                  bool close);

  
  virtual content::WebContents* GetWebContents() const OVERRIDE;
  
  virtual void WillClose(NativeWebContentsModalDialog dialog) OVERRIDE;

  
  class TestApi {
   public:
    explicit TestApi(WebContentsModalDialogManager* manager)
        : manager_(manager) {}

    void CloseAllDialogs() { manager_->CloseAllDialogs(); }
    void DidAttachInterstitialPage() { manager_->DidAttachInterstitialPage(); }
    void ResetNativeManager(NativeWebContentsModalDialogManager* delegate) {
      manager_->native_manager_.reset(delegate);
    }
    void WebContentsWasShown() { manager_->WasShown(); }
    void WebContentsWasHidden() { manager_->WasHidden(); }

   private:
    WebContentsModalDialogManager* manager_;

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

 private:
  explicit WebContentsModalDialogManager(content::WebContents* web_contents);
  friend class content::WebContentsUserData<WebContentsModalDialogManager>;

  struct DialogState {
    explicit DialogState(NativeWebContentsModalDialog dialog);

    NativeWebContentsModalDialog dialog;
    bool close_on_interstitial_webui;
  };

  typedef std::deque<DialogState> WebContentsModalDialogList;

  
  WebContentsModalDialogList::iterator FindDialogState(
      NativeWebContentsModalDialog dialog);

  
  void BlockWebContentsInteraction(bool blocked);

  bool IsWebContentsVisible() const;

  
  void CloseAllDialogs();

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidGetIgnoredUIEvent() OVERRIDE;
  virtual void WasShown() OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual void WebContentsDestroyed(content::WebContents* tab) OVERRIDE;
  virtual void DidAttachInterstitialPage() OVERRIDE;

  
  WebContentsModalDialogManagerDelegate* delegate_;

  
  scoped_ptr<NativeWebContentsModalDialogManager> native_manager_;

  
  WebContentsModalDialogList child_dialogs_;

  
  bool closing_all_dialogs_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsModalDialogManager);
};

}  

#endif  
