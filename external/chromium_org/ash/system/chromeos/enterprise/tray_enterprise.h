// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_ENTERPRISE_TRAY_ENTERPRISE_H
#define ASH_SYSTEM_CHROMEOS_ENTERPRISE_TRAY_ENTERPRISE_H

#include "ash/system/chromeos/enterprise/enterprise_domain_observer.h"
#include "ash/system/tray/system_tray_item.h"
#include "ash/system/tray/view_click_listener.h"

namespace ash {
class SystemTray;
}

namespace ash {
namespace internal {

class LabelTrayView;

class TrayEnterprise : public SystemTrayItem,
                       public ViewClickListener,
                       public EnterpriseDomainObserver {
 public:
  explicit TrayEnterprise(SystemTray* system_tray);
  virtual ~TrayEnterprise();

  
  
  void UpdateEnterpriseMessage();

  
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;

  
  virtual void OnEnterpriseDomainChanged() OVERRIDE;

  
  virtual void OnViewClicked(views::View* sender) OVERRIDE;

 private:
  LabelTrayView* tray_view_;

  DISALLOW_COPY_AND_ASSIGN(TrayEnterprise);
};

} 
} 

#endif  

