// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_CONFIG_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_CONFIG_VIEW_H_
#pragma once

#include "chrome/browser/chromeos/cros/network_library.h"
#include "views/window/dialog_delegate.h"

namespace views {
class View;
class Window;
}

namespace chromeos {

class ChildNetworkConfigView;

class NetworkConfigView : public views::View,
                          public views::DialogDelegate {
 public:
  class Delegate {
   public:
    
    virtual void OnDialogAccepted() = 0;

    
    virtual void OnDialogCancelled() = 0;

   protected:
     virtual ~Delegate() {}
  };

  
  explicit NetworkConfigView(Network* network);
  
  explicit NetworkConfigView(ConnectionType type);
  virtual ~NetworkConfigView() {}

  
  gfx::NativeWindow GetNativeWindow() const;

  
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual bool IsDialogButtonEnabled(
      MessageBoxFlags::DialogButton button) const;
  virtual bool Cancel();
  virtual bool Accept();

  
  virtual bool IsModal() const { return true; }
  virtual views::View* GetContentsView() { return this; }

  
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  void set_browser_mode(bool value) {
    browser_mode_ = value;
  }
  bool is_browser_mode() const {
    return browser_mode_;
  }

  void set_delegate(Delegate* delegate) {
    delegate_ = delegate;
  }

 protected:
  
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

 private:
  
  bool browser_mode_;

  
  
  ChildNetworkConfigView* child_config_view_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigView);
};

class ChildNetworkConfigView : public views::View {
 public:
  
  virtual string16 GetTitle() = 0;

  
  virtual bool CanLogin() = 0;

  
  
  virtual bool Login() = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual void InitFocus() = 0;

  
  static const int kPassphraseWidth;

 protected:
  explicit ChildNetworkConfigView(NetworkConfigView* parent, Network* network)
      : service_path_(network->service_path()),
        parent_(parent) {}
  explicit ChildNetworkConfigView(NetworkConfigView* parent)
      : parent_(parent) {}
  virtual ~ChildNetworkConfigView() {}

  std::string service_path_;
  NetworkConfigView* parent_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChildNetworkConfigView);
};

}  

#endif  
