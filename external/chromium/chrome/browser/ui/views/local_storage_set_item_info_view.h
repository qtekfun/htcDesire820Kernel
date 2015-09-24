// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCAL_STORAGE_SET_ITEM_INFO_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCAL_STORAGE_SET_ITEM_INFO_VIEW_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "views/view.h"

namespace views {
class Label;
class Textfield;
}

class LocalStorageSetItemInfoView : public views::View {
 public:
  LocalStorageSetItemInfoView();
  virtual ~LocalStorageSetItemInfoView();

  
  void SetFields(const std::string& host,
                 const string16& key,
                 const string16& value);

  
  
  void ClearLocalStorageDisplay();

  
  void EnableLocalStorageDisplay(bool enabled);

 protected:
  
  virtual void ViewHierarchyChanged(
      bool is_add, views::View* parent, views::View* child);

 private:
  
  void Init();

  
  views::Textfield* host_value_field_;
  views::Textfield* key_value_field_;
  views::Textfield* value_value_field_;

  DISALLOW_COPY_AND_ASSIGN(LocalStorageSetItemInfoView);
};


#endif  

