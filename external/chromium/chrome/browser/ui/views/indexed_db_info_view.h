// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INDEXED_DB_INFO_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_INDEXED_DB_INFO_VIEW_H_
#pragma once

#include "views/view.h"
#include "chrome/browser/browsing_data_indexed_db_helper.h"

namespace views {
class Label;
class Textfield;
}

class IndexedDBInfoView : public views::View {
 public:
  IndexedDBInfoView();
  virtual ~IndexedDBInfoView();

  
  void SetIndexedDBInfo(
      const BrowsingDataIndexedDBHelper::IndexedDBInfo&
      indexed_db_info);

  
  
  void ClearIndexedDBDisplay();

  
  void EnableIndexedDBDisplay(bool enabled);

 protected:
  
  virtual void ViewHierarchyChanged(
      bool is_add, views::View* parent, views::View* child);

 private:
  
  void Init();

  
  views::Textfield* origin_value_field_;
  views::Textfield* size_value_field_;
  views::Textfield* last_modified_value_field_;

  DISALLOW_COPY_AND_ASSIGN(IndexedDBInfoView);
};

#endif  
