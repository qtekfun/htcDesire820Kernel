// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_VIEW_STORAGE_H_
#define UI_VIEWS_FOCUS_VIEW_STORAGE_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "ui/views/views_export.h"

template <typename T> struct DefaultSingletonTraits;


namespace views {
class View;

class VIEWS_EXPORT ViewStorage {
 public:
  
  
  static ViewStorage* GetInstance();

  
  int CreateStorageID();

  
  void StoreView(int storage_id, View* view);

  
  View* RetrieveView(int storage_id);

  
  void RemoveView(int storage_id);

  
  void ViewRemoved(View* removed);

  size_t view_count() const { return view_to_ids_.size(); }

 private:
  friend struct DefaultSingletonTraits<ViewStorage>;

  ViewStorage();
  ~ViewStorage();

  
  
  void EraseView(int storage_id, bool remove_all_ids);

  
  int view_storage_next_id_;

  
  std::map<int, View*> id_to_view_;

  
  std::map<View*, std::vector<int>*> view_to_ids_;

  DISALLOW_COPY_AND_ASSIGN(ViewStorage);
};

}  

#endif  
