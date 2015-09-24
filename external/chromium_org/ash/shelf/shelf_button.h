// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_BUTTON_H_
#define ASH_SHELF_SHELF_BUTTON_H_

#include "ash/ash_export.h"
#include "ui/gfx/shadow_value.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/image_view.h"

namespace ash {
namespace internal {

class ShelfButtonHost;
class ShelfLayoutManager;

class ASH_EXPORT ShelfButton : public views::CustomButton {
 public:
  
  enum State {
    
    
    STATE_NORMAL    = 0,
    
    STATE_HOVERED   = 1 << 0,
    
    STATE_RUNNING   = 1 << 1,
    
    STATE_ACTIVE    = 1 << 2,
    
    STATE_ATTENTION = 1 << 3,
    STATE_FOCUSED   = 1 << 4,
    
    STATE_HIDDEN = 1 << 5,
  };

  virtual ~ShelfButton();

  
  static ShelfButton* Create(views::ButtonListener* listener,
                             ShelfButtonHost* host,
                             ShelfLayoutManager* shelf_layout_manager);

  
  void SetImage(const gfx::ImageSkia& image);

  
  const gfx::ImageSkia& GetImage() const;

  
  void AddState(State state);
  void ClearState(State state);
  int state() const { return state_; }
  const ShelfLayoutManager* shelf_layout_manager() const {
    return shelf_layout_manager_;
  }

  
  gfx::Rect GetIconBounds() const;

  
  virtual void ShowContextMenu(const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void OnMouseCaptureLost() OVERRIDE;

 protected:
  ShelfButton(views::ButtonListener* listener,
              ShelfButtonHost* host,
              ShelfLayoutManager* shelf_layout_manager);

  
  
  
  class IconView : public views::ImageView {
   public:
    IconView();
    virtual ~IconView();

    void set_icon_size(int icon_size) { icon_size_ = icon_size; }
    int icon_size() const { return icon_size_; }

    
    virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;

   private:
    
    
    int icon_size_;

    DISALLOW_COPY_AND_ASSIGN(IconView);
  };

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  void SetShadowedImage(const gfx::ImageSkia& bitmap);
  
  virtual void Init();
  
  virtual IconView* CreateIconView();
  IconView* icon_view() const { return icon_view_; }
  ShelfButtonHost* host() const { return host_; }

 private:
  class BarView;

  
  
  bool IsShelfHorizontal() const;

  
  
  void UpdateState();

  
  void UpdateBar();

  ShelfButtonHost* host_;
  IconView* icon_view_;
  
  BarView* bar_;
  
  
  int state_;

  ShelfLayoutManager* shelf_layout_manager_;

  gfx::ShadowValues icon_shadows_;

  
  
  bool* destroyed_flag_;

  DISALLOW_COPY_AND_ASSIGN(ShelfButton);
};

}  
}  

#endif  
