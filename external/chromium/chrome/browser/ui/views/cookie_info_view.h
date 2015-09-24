// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_COOKIE_INFO_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_COOKIE_INFO_VIEW_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "net/base/cookie_monster.h"
#include "ui/base/models/combobox_model.h"
#include "views/controls/combobox/combobox.h"
#include "views/view.h"

namespace views {
class GridLayout;
class Label;
class NativeButton;
class Textfield;
}


class CookieInfoViewDelegate {
 public:
  virtual void ModifyExpireDate(bool session_expire) = 0;

 protected:
  virtual ~CookieInfoViewDelegate() {}
};

class CookieInfoView : public views::View,
                       public views::Combobox::Listener,
                       public ui::ComboboxModel {
 public:
  explicit CookieInfoView(bool editable_expiration_date);
  virtual ~CookieInfoView();

  
  void SetCookie(const std::string& domain,
                 const net::CookieMonster::CanonicalCookie& cookie_node);

  
  void SetCookieString(const GURL& url, const std::string& cookie_line);

  
  
  void ClearCookieDisplay();

  
  void EnableCookieDisplay(bool enabled);

  void set_delegate(CookieInfoViewDelegate* delegate) { delegate_ = delegate; }

 protected:
  
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

  
  virtual void ItemChanged(views::Combobox* combo_box,
                           int prev_index,
                           int new_index);

  
  virtual int GetItemCount();
  virtual string16 GetItemAt(int index);

 private:
  
  void AddLabelRow(int layout_id, views::GridLayout* layout,
                   views::View* label, views::View* value);
  void AddControlRow(int layout_id, views::GridLayout* layout,
                     views::View* label, views::View* control);

  
  void Init();

  
  views::Label* name_label_;
  views::Textfield* name_value_field_;
  views::Label* content_label_;
  views::Textfield* content_value_field_;
  views::Label* domain_label_;
  views::Textfield* domain_value_field_;
  views::Label* path_label_;
  views::Textfield* path_value_field_;
  views::Label* send_for_label_;
  views::Textfield* send_for_value_field_;
  views::Label* created_label_;
  views::Textfield* created_value_field_;
  views::Label* expires_label_;
  views::Textfield* expires_value_field_;
  views::Combobox* expires_value_combobox_;
  views::View* expire_view_;

  
  std::vector<std::wstring> expire_combo_values_;

  
  
  
  
  
  bool editable_expiration_date_;

  CookieInfoViewDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CookieInfoView);
};

#endif  

