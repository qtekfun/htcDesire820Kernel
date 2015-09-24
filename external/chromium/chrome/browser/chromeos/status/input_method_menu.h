// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_INPUT_METHOD_MENU_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_INPUT_METHOD_MENU_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/cros/input_method_library.h"
#include "chrome/browser/chromeos/status/status_area_host.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_type.h"
#include "ui/base/models/simple_menu_model.h"
#include "views/controls/menu/menu_2.h"
#include "views/controls/menu/view_menu_delegate.h"

class PrefService;
class SkBitmap;

namespace chromeos {

class InputMethodMenu : public views::ViewMenuDelegate,
                        public ui::MenuModel,
                        public InputMethodLibrary::Observer,
                        public NotificationObserver {
 public:
  InputMethodMenu(PrefService* pref_service,
                  StatusAreaHost::ScreenMode screen_mode,
                  bool for_out_of_box_experience_dialog);
  virtual ~InputMethodMenu();

  
  virtual bool HasIcons() const;
  virtual int GetItemCount() const;
  virtual ui::MenuModel::ItemType GetTypeAt(int index) const;
  virtual int GetCommandIdAt(int index) const;
  virtual string16 GetLabelAt(int index) const;
  virtual bool IsItemDynamicAt(int index) const;
  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const;
  virtual bool IsItemCheckedAt(int index) const;
  virtual int GetGroupIdAt(int index) const;
  virtual bool GetIconAt(int index, SkBitmap* icon);
  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(int index) const;
  virtual bool IsEnabledAt(int index) const;
  virtual ui::MenuModel* GetSubmenuModelAt(int index) const;
  virtual void HighlightChangedTo(int index);
  virtual void ActivatedAt(int index);
  virtual void MenuWillShow();
  virtual void SetMenuModelDelegate(ui::MenuModelDelegate* delegate);

  
  
  virtual void RunMenu(views::View* unused_source,
                       const gfx::Point& pt);

  
  virtual void InputMethodChanged(
      InputMethodLibrary* obj,
      const InputMethodDescriptor& current_input_method,
      size_t num_active_input_methods);
  virtual void ActiveInputMethodsChanged(
      InputMethodLibrary* obj,
      const InputMethodDescriptor& current_input_method,
      size_t num_active_input_methods);
  virtual void PreferenceUpdateNeeded(
    InputMethodLibrary* obj,
    const InputMethodDescriptor& previous_input_method,
    const InputMethodDescriptor& current_input_method);
  virtual void PropertyListChanged(
      InputMethodLibrary* obj,
      const ImePropertyList& current_ime_properties);
  virtual void FirstObserverIsAdded(InputMethodLibrary* obj);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void SetMinimumWidth(int width);

  
  void PrepareMenu();

  
  static void RegisterPrefs(PrefService* local_state);

  
  
  static std::wstring GetTextForIndicator(
      const InputMethodDescriptor& input_method);

  
  
  static std::wstring GetTextForMenu(const InputMethodDescriptor& input_method);

 protected:
  
  void PrepareForMenuOpen();

  
  views::Menu2& input_method_menu() {
    return input_method_menu_;
  }

 private:
  
  
  virtual void UpdateUI(const std::string& input_method_id,  
                        const std::wstring& name,  
                        const std::wstring& tooltip,
                        size_t num_active_input_methods) = 0;

  
  
  
  virtual bool ShouldSupportConfigUI() = 0;

  
  
  virtual void OpenConfigUI() = 0;

  
  void UpdateUIFromInputMethod(const InputMethodDescriptor& input_method,
                               size_t num_active_input_methods);

  
  
  
  void RebuildModel();

  
  bool IndexIsInInputMethodList(int index) const;

  
  
  
  bool GetPropertyIndex(int index, int* property_index) const;

  
  
  bool IndexPointsToConfigureImeMenuItem(int index) const;

  
  scoped_ptr<InputMethodDescriptors> input_method_descriptors_;

  
  StringPrefMember previous_input_method_pref_;
  StringPrefMember current_input_method_pref_;

  
  
  
  scoped_ptr<ui::SimpleMenuModel> model_;

  
  views::Menu2 input_method_menu_;
  int minimum_input_method_menu_width_;

  PrefService* pref_service_;
  NotificationRegistrar registrar_;

  
  const StatusAreaHost::ScreenMode screen_mode_;
  
  
  const bool for_out_of_box_experience_dialog_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodMenu);
};

}  

#endif  
