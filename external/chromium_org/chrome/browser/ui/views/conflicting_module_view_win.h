// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CONFLICTING_MODULE_VIEW_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_CONFLICTING_MODULE_VIEW_WIN_H_

#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "url/gurl.h"

class Browser;

namespace views {
class Label;
class LabelButton;
}

class ConflictingModuleView : public views::BubbleDelegateView,
                              public views::ButtonListener,
                              public content::NotificationObserver {
 public:
  ConflictingModuleView(views::View* anchor_view,
                        Browser* browser,
                        const GURL& help_center_url);

  
  static void MaybeShow(Browser* browser, views::View* anchor_view);

 private:
  virtual ~ConflictingModuleView();

  
  void ShowBubble();

  
  
  void DismissBubble();

  
  virtual void Init() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  virtual void Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) OVERRIDE;

  Browser* browser_;

  content::NotificationRegistrar registrar_;

  
  views::Label* explanation_;
  views::LabelButton* learn_more_button_;
  views::LabelButton* not_now_button_;

  
  GURL help_center_url_;

  DISALLOW_COPY_AND_ASSIGN(ConflictingModuleView);
};

#endif  
