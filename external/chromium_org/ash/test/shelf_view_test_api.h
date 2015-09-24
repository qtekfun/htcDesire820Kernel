// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_SHELF_VIEW_TEST_API_H_
#define ASH_TEST_SHELF_VIEW_TEST_API_H_

#include "ash/launcher/launcher_types.h"
#include "base/basictypes.h"

namespace gfx {
class Rect;
class Size;
}

namespace ash {

class ShelfDelegate;

namespace internal {
class OverflowBubble;
class ShelfButton;
class ShelfView;
}

namespace test {

class ShelfViewTestAPI {
 public:
  explicit ShelfViewTestAPI(internal::ShelfView* shelf_view);
  ~ShelfViewTestAPI();

  
  int GetButtonCount();

  
  internal::ShelfButton* GetButton(int index);

  
  int GetFirstVisibleIndex();

  
  int GetLastVisibleIndex();

  
  const gfx::Rect& GetBoundsByIndex(int index);
  const gfx::Rect& GetIdealBoundsByIndex(int index);

  
  bool IsOverflowButtonVisible();

  
  void ShowOverflowBubble();

  
  void SetAnimationDuration(int duration_ms);

  
  void RunMessageLoopUntilAnimationsDone();

  
  internal::ShelfView* shelf_view() { return shelf_view_; }

  
  internal::OverflowBubble* overflow_bubble();

  
  gfx::Size GetPreferredSize();

  
  int GetButtonSize();

  
  int GetButtonSpacing();

  
  bool SameDragType(LauncherItemType typea, LauncherItemType typeb) const;

  
  void SetShelfDelegate(ShelfDelegate* delegate);

  
  gfx::Rect GetBoundsForDragInsertInScreen();

  
  bool IsRippedOffFromShelf();

  
  bool DraggedItemFromOverflowToShelf();

 private:
  internal::ShelfView* shelf_view_;

  DISALLOW_COPY_AND_ASSIGN(ShelfViewTestAPI);
};

}  
}  

#endif  
