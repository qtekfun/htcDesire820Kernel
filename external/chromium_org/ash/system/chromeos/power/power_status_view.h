// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_POWER_STATUS_VIEW_H_
#define ASH_SYSTEM_CHROMEOS_POWER_POWER_STATUS_VIEW_H_

#include "ash/system/chromeos/power/power_status.h"
#include "ui/views/view.h"

namespace views {
class ImageView;
class Label;
}

namespace ash {
namespace internal {

class PowerStatusView : public views::View, public PowerStatus::Observer {
 public:
  enum ViewType {
    VIEW_DEFAULT,
    VIEW_NOTIFICATION
  };

  PowerStatusView(ViewType view_type, bool default_view_right_align);
  virtual ~PowerStatusView();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual void OnPowerStatusChanged() OVERRIDE;

 private:
  void LayoutDefaultView();
  void LayoutNotificationView();
  void UpdateTextForDefaultView();
  void UpdateTextForNotificationView();

  
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;

  
  
  bool default_view_right_align_;

  
  views::Label* status_label_;
  views::Label* time_label_;

  
  views::Label* time_status_label_;
  views::Label* percentage_label_;

  
  views::ImageView* icon_;

  ViewType view_type_;

  DISALLOW_COPY_AND_ASSIGN(PowerStatusView);
};

}  
}  

#endif  
