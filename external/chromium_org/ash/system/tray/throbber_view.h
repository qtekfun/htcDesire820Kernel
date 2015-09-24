// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_THROBBER_VIEW_H_
#define ASH_SYSTEM_TRAY_THROBBER_VIEW_H_

#include "ui/gfx/size.h"
#include "ui/views/controls/throbber.h"
#include "ui/views/view.h"

namespace ash {
namespace internal {

class SystemTrayThrobber : public views::SmoothedThrobber {
 public:
  SystemTrayThrobber(int frame_delay_ms);
  virtual ~SystemTrayThrobber();

  void SetTooltipText(const base::string16& tooltip_text);

  
  virtual bool GetTooltipText(
        const gfx::Point& p, base::string16* tooltip) const OVERRIDE;

 private:
  
  base::string16 tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(SystemTrayThrobber);
};

class ThrobberView : public views::View {
 public:
  ThrobberView();
  virtual ~ThrobberView();

  void Start();
  void Stop();
  void SetTooltipText(const base::string16& tooltip_text);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool GetTooltipText(
      const gfx::Point& p, base::string16* tooltip) const OVERRIDE;

 private:
  
  void ScheduleAnimation(bool start_throbber);

  SystemTrayThrobber* throbber_;

  
  base::string16 tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(ThrobberView);
};

}  
}  

#endif  
