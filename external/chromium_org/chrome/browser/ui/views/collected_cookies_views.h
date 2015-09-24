// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_COLLECTED_COOKIES_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_COLLECTED_COOKIES_VIEWS_H_

#include "base/compiler_specific.h"
#include "chrome/common/content_settings.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane_listener.h"
#include "ui/views/controls/tree/tree_view_controller.h"
#include "ui/views/window/dialog_delegate.h"

class CookieInfoView;
class CookiesTreeModel;
class InfobarView;

namespace content {
class WebContents;
}

namespace views {
class Label;
class LabelButton;
class TreeView;
class Widget;
}

class CollectedCookiesViews : public views::DialogDelegateView,
                              public content::NotificationObserver,
                              public views::ButtonListener,
                              public views::TabbedPaneListener,
                              public views::TreeViewController {
 public:
  
  explicit CollectedCookiesViews(content::WebContents* web_contents);

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual int GetDialogButtons() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void TabSelectedAt(int index) OVERRIDE;

  
  virtual void OnTreeViewSelectionChanged(views::TreeView* tree_view) OVERRIDE;

  
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  virtual ~CollectedCookiesViews();

  void Init();

  views::View* CreateAllowedPane();

  views::View* CreateBlockedPane();

  
  views::View* CreateScrollView(views::TreeView* pane);

  void EnableControls();

  void ShowCookieInfo();

  void AddContentException(views::TreeView* tree_view, ContentSetting setting);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  views::Widget* window_;

  
  content::WebContents* web_contents_;

  
  views::Label* allowed_label_;
  views::Label* blocked_label_;

  views::TreeView* allowed_cookies_tree_;
  views::TreeView* blocked_cookies_tree_;

  views::LabelButton* block_allowed_button_;
  views::LabelButton* allow_blocked_button_;
  views::LabelButton* for_session_blocked_button_;

  scoped_ptr<CookiesTreeModel> allowed_cookies_tree_model_;
  scoped_ptr<CookiesTreeModel> blocked_cookies_tree_model_;

  CookieInfoView* cookie_info_view_;

  InfobarView* infobar_;

  bool status_changed_;

  DISALLOW_COPY_AND_ASSIGN(CollectedCookiesViews);
};

#endif  
