// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_LAUNCHER_LAUNCHER_H_
#define ASH_LAUNCHER_LAUNCHER_H_

#include "ash/ash_export.h"
#include "ash/launcher/launcher_types.h"
#include "ash/shelf/shelf_types.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/size.h"
#include "ui/views/widget/widget_observer.h"

namespace app_list {
class ApplicationDragAndDropHost;
}

namespace aura {
class Window;
}

namespace gfx {
class Rect;
}

namespace views {
class View;
}

namespace ash {

namespace internal {
class FocusCycler;
class ShelfLayoutManager;
class ShelfView;
}

namespace test {
class LauncherTestAPI;
}

class ShelfDelegate;
class ShelfIconObserver;
class ShelfModel;
class ShelfWidget;

class ASH_EXPORT Launcher {
 public:
  static const char kNativeViewName[];

  Launcher(ShelfModel* model, ShelfDelegate* delegate, ShelfWidget* widget);
  virtual ~Launcher();

  
  
  static Launcher* ForPrimaryDisplay();

  
  
  
  static Launcher* ForWindow(aura::Window* window);

  void SetAlignment(ShelfAlignment alignment);
  ShelfAlignment alignment() const { return alignment_; }

  
  
  gfx::Rect GetScreenBoundsOfItemIconForWindow(aura::Window* window);

  
  
  void UpdateIconPositionForWindow(aura::Window* window);

  
  
  void ActivateLauncherItem(int index);

  
  void CycleWindowLinear(CycleDirection direction);

  void AddIconObserver(ShelfIconObserver* observer);
  void RemoveIconObserver(ShelfIconObserver* observer);

  
  bool IsShowingMenu() const;

  bool IsShowingOverflowBubble() const;

  void SetVisible(bool visible) const;
  bool IsVisible() const;

  void SchedulePaint();

  views::View* GetAppListButtonView() const;

  
  
  void LaunchAppIndexAt(int item_index);

  ShelfWidget* shelf_widget() { return shelf_widget_; }

  
  void SetShelfViewBounds(gfx::Rect bounds);
  gfx::Rect GetShelfViewBounds() const;

  
  
  gfx::Rect GetVisibleItemsBoundsInScreen() const;

  
  app_list::ApplicationDragAndDropHost* GetDragAndDropHostForAppList();

 private:
  friend class ash::test::LauncherTestAPI;

  
  internal::ShelfView* shelf_view_;

  ShelfAlignment alignment_;

  ShelfDelegate* delegate_;

  ShelfWidget* shelf_widget_;

  DISALLOW_COPY_AND_ASSIGN(Launcher);
};

}  

#endif  
