// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCESSIBILITY_DELEGATE_H_
#define ASH_ACCESSIBILITY_DELEGATE_H_

#include "ash/ash_export.h"
#include "ash/magnifier/magnifier_constants.h"
#include "base/time/time.h"

namespace ash {

enum AccessibilityNotificationVisibility {
  A11Y_NOTIFICATION_NONE,
  A11Y_NOTIFICATION_SHOW,
};

enum AccessibilityAlert {
  A11Y_ALERT_NONE,
  A11Y_ALERT_WINDOW_NEEDED
};

class ASH_EXPORT AccessibilityDelegate {
 public:
  virtual ~AccessibilityDelegate() {}

  
  virtual void ToggleSpokenFeedback(
      AccessibilityNotificationVisibility notify) = 0;

  
  virtual bool IsSpokenFeedbackEnabled() const = 0;

  
  virtual void ToggleHighContrast() = 0;

  
  virtual bool IsHighContrastEnabled() const = 0;

  
  virtual void SetMagnifierEnabled(bool enabled) = 0;

  
  virtual void SetMagnifierType(MagnifierType type) = 0;

  
  virtual bool IsMagnifierEnabled() const = 0;

  
  virtual MagnifierType GetMagnifierType() const = 0;

  
  virtual void SetLargeCursorEnabled(bool enabled) = 0;

  
  virtual bool IsLargeCursorEnabled() const = 0;

  
  virtual void SetAutoclickEnabled(bool enabled) = 0;

  
  virtual bool IsAutoclickEnabled() const = 0;

  
  virtual bool ShouldShowAccessibilityMenu() const = 0;

  
  virtual void SilenceSpokenFeedback() const = 0;

  
  virtual void SaveScreenMagnifierScale(double scale) = 0;

  
  
  virtual double GetSavedScreenMagnifierScale() = 0;

  
  virtual void TriggerAccessibilityAlert(AccessibilityAlert alert) = 0;

  
  virtual AccessibilityAlert GetLastAccessibilityAlert() = 0;

  
  virtual base::TimeDelta PlayShutdownSound() const = 0;
};

}  

#endif  
