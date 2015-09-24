// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_NOTIFICATION_VIEW_H_
#define ASH_SYSTEM_TRAY_TRAY_NOTIFICATION_VIEW_H_

#include "base/timer/timer.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/slide_out_view.h"

namespace gfx {
class ImageSkia;
}

namespace views {
class ImageView;
}

namespace ash {

class SystemTrayItem;

namespace internal {

class TrayNotificationView : public views::SlideOutView,
                             public views::ButtonListener {
 public:
  
  
  TrayNotificationView(SystemTrayItem* owner, int icon_id);
  virtual ~TrayNotificationView();

  
  void InitView(views::View* contents);

  
  void SetIconImage(const gfx::ImageSkia& image);

  
  const gfx::ImageSkia& GetIconImage() const;

  
  void UpdateView(views::View* new_contents);

  
  void UpdateViewAndImage(views::View* new_contents,
                          const gfx::ImageSkia& image);

  
  void StartAutoCloseTimer(int seconds);
  void StopAutoCloseTimer();
  void RestartAutoCloseTimer();

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 protected:
  
  virtual void OnClose();
  
  virtual void OnClickAction();

  
  virtual void OnSlideOut() OVERRIDE;

  SystemTrayItem* owner() { return owner_; }

 private:
  void HandleClose();
  void HandleClickAction();

  SystemTrayItem* owner_;
  int icon_id_;
  views::ImageView* icon_;

  int autoclose_delay_;
  base::OneShotTimer<TrayNotificationView> autoclose_;

  DISALLOW_COPY_AND_ASSIGN(TrayNotificationView);
};

}  
}  

#endif  
