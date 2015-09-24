// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SIMPLE_WEB_VIEW_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SIMPLE_WEB_VIEW_DIALOG_H_

#include <string>
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/command_updater_delegate.h"
#include "chrome/browser/ui/toolbar/toolbar_model_delegate.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/widget/widget_delegate.h"
#include "url/gurl.h"

class CommandUpdater;
class Profile;
class ReloadButton;
class ToolbarModel;

namespace views {
class WebView;
class Widget;
}

namespace chromeos {

class StubBubbleModelDelegate;

class SimpleWebViewDialog : public views::ButtonListener,
                            public views::WidgetDelegateView,
                            public LocationBarView::Delegate,
                            public ToolbarModelDelegate,
                            public CommandUpdaterDelegate,
                            public content::PageNavigator,
                            public content::WebContentsDelegate {
 public:
  explicit SimpleWebViewDialog(Profile* profile);
  virtual ~SimpleWebViewDialog();

  
  void StartLoad(const GURL& gurl);

  
  void Init();

  
  virtual void Layout() OVERRIDE;

  
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual content::WebContents* OpenURL(
      const content::OpenURLParams& params) OVERRIDE;

  
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;

  
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual ToolbarModel* GetToolbarModel() OVERRIDE;
  virtual const ToolbarModel* GetToolbarModel() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual views::Widget* CreateViewsBubble(
      views::BubbleDelegateView* bubble_delegate) OVERRIDE;
  virtual PageActionImageView* CreatePageActionImageView(
      LocationBarView* owner,
      ExtensionAction* action) OVERRIDE;
  virtual ContentSettingBubbleModelDelegate*
  GetContentSettingBubbleModelDelegate() OVERRIDE;
  virtual void ShowWebsiteSettings(content::WebContents* web_contents,
                                   const GURL& url,
                                   const content::SSLStatus& ssl) OVERRIDE;

  
  virtual content::WebContents* GetActiveWebContents() const OVERRIDE;

  
  virtual void ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition) OVERRIDE;

 private:
  void LoadImages();
  void UpdateButtons();
  void UpdateReload(bool is_loading, bool force);

  Profile* profile_;
  scoped_ptr<ToolbarModel> toolbar_model_;
  scoped_ptr<CommandUpdater> command_updater_;

  
  views::ImageButton* back_;
  views::ImageButton* forward_;
  ReloadButton* reload_;
  LocationBarView* location_bar_;
  views::WebView* web_view_;

  
  scoped_ptr<views::WebView> web_view_container_;

  scoped_ptr<StubBubbleModelDelegate> bubble_model_delegate_;

  DISALLOW_COPY_AND_ASSIGN(SimpleWebViewDialog);
};

}  

#endif  
