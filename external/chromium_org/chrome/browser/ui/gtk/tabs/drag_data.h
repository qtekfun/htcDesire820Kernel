// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_DRAG_DATA_H_
#define CHROME_BROWSER_UI_GTK_TABS_DRAG_DATA_H_

#include <vector>

#include "base/basictypes.h"

class TabGtk;

namespace content {
class WebContents;
class WebContentsDelegate;
}

struct DraggedTabData {
 public:
  DraggedTabData();
  DraggedTabData(TabGtk* tab,
                 content::WebContents* contents,
                 content::WebContentsDelegate* original_delegate,
                 int source_model_index,
                 bool pinned,
                 bool mini);
  ~DraggedTabData();

  
  void ResetDelegate();

  
  TabGtk* tab_;

  
  content::WebContents* contents_;

  
  
  
  content::WebContentsDelegate* original_delegate_;

  
  
  int source_model_index_;

  
  bool pinned_;

  
  bool mini_;
};

class DragData {
 public:
  DragData(std::vector<DraggedTabData> drag_data, int source_tab_index);
  ~DragData();

  
  std::vector<TabGtk*> GetDraggedTabs() const;

  
  std::vector<content::WebContents*> GetDraggedTabsContents() const;

  
  
  int GetAddTypesForDraggedTabAt(size_t index);

  
  
  
  void GetNumberOfMiniNonMiniTabs(int from, int to, int* mini,
                                  int* non_mini) const;

  
  size_t size() const { return drag_data_.size(); }

  
  
  DraggedTabData* get(size_t index) { return &drag_data_[index]; }

  int source_tab_index() const { return source_tab_index_; }
  int mini_tab_count() const { return mini_tab_count_; }
  int non_mini_tab_count() const { return non_mini_tab_count_; }

  
  content::WebContents* GetSourceWebContents();

  
  
  DraggedTabData* GetSourceTabData();

 private:
  std::vector<DraggedTabData> drag_data_;

  
  int source_tab_index_;
  
  int non_mini_tab_count_;
  
  int mini_tab_count_;

  DISALLOW_COPY_AND_ASSIGN(DragData);
};

#endif  
