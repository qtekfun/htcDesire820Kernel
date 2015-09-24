// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_DROP_INFO_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_DROP_INFO_H_
#pragma once

#include "base/basictypes.h"
#include "base/timer.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "ui/gfx/native_widget_types.h"

namespace views {
class DropTargetEvent;
}

class BookmarkDropInfo {
 public:
  BookmarkDropInfo(gfx::NativeWindow hwnd, int top_margin);
  virtual ~BookmarkDropInfo();

  
  
  void Update(const views::DropTargetEvent& event);

  
  void SetData(const BookmarkNodeData& data) { data_ = data; }
  BookmarkNodeData& data() { return data_; }

  
  int source_operations() const { return source_operations_; }

  
  bool is_control_down() const { return is_control_down_; }

  
  int last_y() { return last_y_; }

  
  
  void set_drop_operation(int drop_operation) {
    drop_operation_ = drop_operation;
  }
  int drop_operation() const { return drop_operation_; }

 protected:
  
  
  
  virtual void Scrolled() = 0;

 private:
  
  void Scroll();

  BookmarkNodeData data_;

  int source_operations_;

  bool is_control_down_;

  int last_y_;

  int drop_operation_;

  gfx::NativeWindow wnd_;

  
  
  int top_margin_;

  
  bool scroll_up_;

  
  base::RepeatingTimer<BookmarkDropInfo> scroll_timer_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkDropInfo);
};

#endif  
