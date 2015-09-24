// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_EXTENSION_TRACKER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_EXTENSION_TRACKER_H_
#pragma once

#include "chrome/browser/automation/automation_resource_tracker.h"

class Extension;

class AutomationExtensionTracker
    : public AutomationResourceTracker<const Extension*> {
 public:
  explicit AutomationExtensionTracker(IPC::Message::Sender* automation);

  virtual ~AutomationExtensionTracker();

  
  
  
  
  
  virtual void AddObserver(const Extension* resource);

  
  virtual void RemoveObserver(const Extension* resource);

  
  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
};

#endif  
