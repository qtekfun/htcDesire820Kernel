// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_NETWORK_SELECTION_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_NETWORK_SELECTION_VIEW_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/chromeos/login/login_html_dialog.h"
#include "chrome/browser/chromeos/views/dropdown_button.h"
#include "views/controls/link.h"
#include "views/view.h"
#include "views/widget/widget_gtk.h"
#include "views/window/window_delegate.h"

namespace gfx {
class Size;
}  

namespace views {
class Combobox;
class GridLayout;
class Label;
class NativeButton;
class Throbber;
}  

namespace chromeos {

class NetworkDropdownButton;
class NetworkScreenDelegate;
class ScreenObserver;

class NetworkSelectionView : public views::View,
                             public views::LinkController,
                             public LoginHtmlDialog::Delegate {
 public:
  explicit NetworkSelectionView(NetworkScreenDelegate* delegate);
  virtual ~NetworkSelectionView();

  
  void Init();

  
  void UpdateLocalizedStringsAndFonts();

  
  gfx::NativeWindow GetNativeWindow() const;

  
  views::View* GetNetworkControlView() const;

  
  void ShowConnectingStatus(bool connecting, const string16& network_id);

  
  bool IsConnecting() const;

  
  void EnableContinue(bool enabled);

  
  bool IsContinueEnabled() const;

  
  virtual void LinkActivated(views::Link* source, int);

  
  bool is_dialog_open() const {
    return proxy_settings_dialog_.get() && proxy_settings_dialog_->is_open();
  }

 protected:
  
  virtual bool OnKeyPressed(const views::KeyEvent& e);
  virtual void OnLocaleChanged();
  virtual bool SkipDefaultKeyEventProcessing(const views::KeyEvent& e) {
    return true;
  }

  
  virtual void OnDialogClosed() {}

 private:
  
  
  
  void AddControlsToLayout(views::GridLayout* contents_layout);

  
  void InitLayout();

  
  
  void RecreateNativeControls();

  
  void UpdateConnectingNetworkLabel();

  
  views::View* entire_screen_view_;

  
  views::View* contents_view_;

  
  DropDownButton* languages_menubutton_;
  DropDownButton* keyboards_menubutton_;
  views::Label* welcome_label_;
  views::Label* select_language_label_;
  views::Label* select_keyboard_label_;
  views::Label* select_network_label_;
  views::Label* connecting_network_label_;
  NetworkDropdownButton* network_dropdown_;
  views::NativeButton* continue_button_;
  views::Throbber* throbber_;
  views::Link* proxy_settings_link_;
  bool show_keyboard_button_;

  
  NetworkScreenDelegate* delegate_;

  
  string16 network_id_;

  
  scoped_ptr<LoginHtmlDialog> proxy_settings_dialog_;

  DISALLOW_COPY_AND_ASSIGN(NetworkSelectionView);
};

}  

#endif  
