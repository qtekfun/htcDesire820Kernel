// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_TRAY_DISPLAY_H_
#define ASH_SYSTEM_CHROMEOS_TRAY_DISPLAY_H_

#include <map>

#include "ash/ash_export.h"
#include "ash/display/display_controller.h"
#include "ash/display/display_info.h"
#include "ash/system/tray/system_tray_item.h"
#include "base/strings/string16.h"
#include "ui/views/view.h"

namespace ash {
namespace test {
class AshTestBase;
}

namespace internal {
class DisplayView;

class ASH_EXPORT TrayDisplay : public SystemTrayItem,
                               public DisplayController::Observer {
 public:
  explicit TrayDisplay(SystemTray* system_tray);
  virtual ~TrayDisplay();

  
  virtual void OnDisplayConfigurationChanged() OVERRIDE;

 private:
  friend class TrayDisplayTest;

  typedef std::map<int64, DisplayInfo> DisplayInfoMap;

  static const char kNotificationId[];

  
  
  void UpdateDisplayInfo(DisplayInfoMap* old_info);

  
  
  
  
  
  
  bool GetDisplayMessageForNotification(
      const DisplayInfoMap& old_info,
      base::string16* message_out,
      base::string16* additional_message_out);

  
  void CreateOrUpdateNotification(const base::string16& message,
                                  const base::string16& additional_message);

  
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;

  
  base::string16 GetDefaultViewMessage() const;
  const views::View* default_view() const {
    return reinterpret_cast<views::View*>(default_);
  }

  DisplayView* default_;
  DisplayInfoMap display_info_;

  DISALLOW_COPY_AND_ASSIGN(TrayDisplay);
};

}  
}  

#endif  
