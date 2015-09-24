// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_DATE_DATE_VIEW_H_
#define ASH_SYSTEM_DATE_DATE_VIEW_H_

#include "ash/ash_export.h"
#include "ash/system/date/tray_date.h"
#include "ash/system/tray/actionable_view.h"
#include "base/i18n/time_formatting.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "ui/views/view.h"

namespace views {
class Label;
}

namespace ash {
namespace internal {
namespace tray {

class ASH_EXPORT BaseDateTimeView : public ActionableView {
 public:
  virtual ~BaseDateTimeView();

  
  void UpdateText();

 protected:
  BaseDateTimeView();

 private:
  
  void SetTimer(const base::Time& now);

  
  virtual void UpdateTextInternal(const base::Time& now) = 0;

  
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
  virtual void OnLocaleChanged() OVERRIDE;

  
  base::OneShotTimer<BaseDateTimeView> timer_;

  DISALLOW_COPY_AND_ASSIGN(BaseDateTimeView);
};

class DateView : public BaseDateTimeView {
 public:
  DateView();
  virtual ~DateView();

  
  
  
  void SetActionable(bool actionable);

  
  void UpdateTimeFormat();

 private:
  
  virtual void UpdateTextInternal(const base::Time& now) OVERRIDE;

  
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;

  
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  views::Label* date_label_;

  
  base::HourClockType hour_type_;

  bool actionable_;

  DISALLOW_COPY_AND_ASSIGN(DateView);
};

class ASH_EXPORT TimeView : public BaseDateTimeView {
 public:
  explicit TimeView(TrayDate::ClockLayout clock_layout);
  virtual ~TimeView();

  
  void UpdateTimeFormat();

  
  void UpdateClockLayout(TrayDate::ClockLayout clock_layout);

 private:
  friend class TimeViewTest;

  
  virtual void UpdateTextInternal(const base::Time& now) OVERRIDE;

  
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;

  void SetBorder(TrayDate::ClockLayout clock_layout);
  void SetupLabels();
  void SetupLabel(views::Label* label);

  
  scoped_ptr<views::Label> horizontal_label_;

  
  scoped_ptr<views::Label> vertical_label_hours_;
  scoped_ptr<views::Label> vertical_label_minutes_;

  base::HourClockType hour_type_;

  DISALLOW_COPY_AND_ASSIGN(TimeView);
};

}  
}  
}  

#endif  
