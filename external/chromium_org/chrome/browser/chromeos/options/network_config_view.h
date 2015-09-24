// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_CONFIG_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_OPTIONS_NETWORK_CONFIG_VIEW_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"  
#include "ui/views/controls/button/button.h"  
#include "ui/views/window/dialog_delegate.h"

namespace gfx {
class ImageSkia;
}

namespace views {
class ImageView;
class LabelButton;
}

namespace chromeos {

class ChildNetworkConfigView;
class NetworkPropertyUIData;
class NetworkState;

class NetworkConfigView : public views::DialogDelegateView,
                          public views::ButtonListener {
 public:
  class Delegate {
   public:
    
    virtual void OnDialogAccepted() = 0;

    
    virtual void OnDialogCancelled() = 0;

   protected:
     virtual ~Delegate() {}
  };

  
  static void Show(const std::string& service_path, gfx::NativeWindow parent);
  
  
  static void ShowForType(const std::string& type, gfx::NativeWindow parent);

  
  gfx::NativeWindow GetNativeWindow() const;

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual views::View* CreateExtraView() OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;

  
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ButtonPressed(
      views::Button* sender, const ui::Event& event) OVERRIDE;

  void set_delegate(Delegate* delegate) {
    delegate_ = delegate;
  }

 protected:
  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

 private:
  NetworkConfigView();
  virtual ~NetworkConfigView();

  
  bool InitWithNetworkState(const NetworkState* network);
  
  bool InitWithType(const std::string& type);

  
  void ShowDialog(gfx::NativeWindow parent);

  
  void ShowAdvancedView();

  
  
  ChildNetworkConfigView* child_config_view_;

  Delegate* delegate_;

  
  views::LabelButton* advanced_button_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigView);
};

class ChildNetworkConfigView : public views::View {
 public:
  
  
  ChildNetworkConfigView(NetworkConfigView* parent,
                         const std::string& service_path);
  virtual ~ChildNetworkConfigView();

  
  virtual base::string16 GetTitle() const = 0;

  
  virtual views::View* GetInitiallyFocusedView() = 0;

  
  virtual bool CanLogin() = 0;

  
  
  virtual bool Login() = 0;

  
  virtual void Cancel() = 0;

  
  
  virtual void InitFocus() = 0;

  
  static const int kInputFieldMinWidth;

 protected:
  NetworkConfigView* parent_;
  std::string service_path_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChildNetworkConfigView);
};

class ControlledSettingIndicatorView : public views::View {
 public:
  ControlledSettingIndicatorView();
  explicit ControlledSettingIndicatorView(const NetworkPropertyUIData& ui_data);
  virtual ~ControlledSettingIndicatorView();

  
  void Update(const NetworkPropertyUIData& ui_data);

 protected:
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  
  void Init();

  bool managed_;
  views::ImageView* image_view_;
  const gfx::ImageSkia* image_;

  DISALLOW_COPY_AND_ASSIGN(ControlledSettingIndicatorView);
};

}  

#endif  
