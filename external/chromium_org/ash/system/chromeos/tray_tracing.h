// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRACING_H_
#define ASH_SYSTEM_TRAY_TRACING_H_

#include "ash/ash_export.h"
#include "ash/system/tray/tray_image_item.h"

namespace views {
class View;
}

namespace ash {

class ASH_EXPORT TracingObserver {
 public:
  virtual ~TracingObserver() {}

  
  virtual void OnTracingModeChanged(bool value) = 0;
};

namespace internal {

class TrayTracing : public TrayImageItem,
                    public TracingObserver {
 public:
  explicit TrayTracing(SystemTray* system_tray);
  virtual ~TrayTracing();

 private:
  void SetTrayIconVisible(bool visible);

  
  virtual bool GetInitialVisibility() OVERRIDE;
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDetailedView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyDetailedView() OVERRIDE;

  
  virtual void OnTracingModeChanged(bool value) OVERRIDE;

  views::View* default_;

  DISALLOW_COPY_AND_ASSIGN(TrayTracing);
};

}  
}  

#endif  
