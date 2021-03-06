// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_SUSPICIOUS_EXTENSION_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_SUSPICIOUS_EXTENSION_BUBBLE_VIEW_H_

#include "chrome/browser/extensions/suspicious_extension_bubble.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"

class Browser;

namespace views {
class Label;
class LabelButton;
class Link;
class Widget;
}

namespace extensions {

class SuspiciousExtensionBubbleController;

class SuspiciousExtensionBubbleView : public SuspiciousExtensionBubble,
                                      public views::BubbleDelegateView,
                                      public views::ButtonListener,
                                      public views::LinkListener {
 public:
  
  static void MaybeShow(Browser* browser, views::View* anchor_view);

  
  virtual void OnButtonClicked(const base::Closure& callback) OVERRIDE;
  virtual void OnLinkClicked(const base::Closure& callback) OVERRIDE;
  virtual void Show() OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;

 private:
  SuspiciousExtensionBubbleView(
      views::View* anchor_view,
      SuspiciousExtensionBubbleController* controller);
  virtual ~SuspiciousExtensionBubbleView();

  
  void ShowBubble();

  
  virtual void Init() OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  base::WeakPtrFactory<SuspiciousExtensionBubbleView> weak_factory_;

  
  SuspiciousExtensionBubbleController* controller_;

  
  views::Label* headline_;
  views::Link* learn_more_;
  views::LabelButton* dismiss_button_;

  
  
  bool link_clicked_;

  
  base::Closure button_callback_;
  base::Closure link_callback_;

  DISALLOW_COPY_AND_ASSIGN(SuspiciousExtensionBubbleView);
};

}  

#endif  
