// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DROP_HELPER_H_
#define UI_VIEWS_WIDGET_DROP_HELPER_H_

#include "base/basictypes.h"

namespace gfx {
class Point;
}  

namespace ui {
class OSExchangeData;
}  
using ui::OSExchangeData;

namespace views {

class RootView;
class View;

class DropHelper {
 public:
  explicit DropHelper(View* root_view);
  ~DropHelper();

  
  View* target_view() const { return target_view_; }

  
  View* root_view() const { return root_view_; }

  
  
  
  
  void ResetTargetViewIfEquals(View* view);

  
  
  
  int OnDragOver(const OSExchangeData& data,
                 const gfx::Point& root_view_location,
                 int drag_operation);

  
  
  void OnDragExit();

  
  
  
  
  
  int OnDrop(const OSExchangeData& data,
             const gfx::Point& root_view_location,
             int drag_operation);

  
  
  
  
  View* CalculateTargetView(const gfx::Point& root_view_location,
                            const OSExchangeData& data,
                            bool check_can_drop);

 private:
  
  
  
  View* CalculateTargetViewImpl(const gfx::Point& root_view_location,
                                const OSExchangeData& data,
                                bool check_can_drop,
                                View** deepest_view);

  
  
  void NotifyDragEntered(const OSExchangeData& data,
                         const gfx::Point& root_view_location,
                         int drag_operation);
  int NotifyDragOver(const OSExchangeData& data,
                     const gfx::Point& root_view_location,
                     int drag_operation);
  void NotifyDragExit();

  
  View* root_view_;

  
  View* target_view_;

  
  View* deepest_view_;

  DISALLOW_COPY_AND_ASSIGN(DropHelper);
};

}  

#endif  
