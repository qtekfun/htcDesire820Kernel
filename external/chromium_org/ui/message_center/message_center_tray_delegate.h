// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_DELEGATE_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_TRAY_DELEGATE_H_

#include "ui/message_center/message_center_export.h"

namespace message_center {

class MessageCenterTray;

class MESSAGE_CENTER_EXPORT MessageCenterTrayDelegate {
 public:
  virtual ~MessageCenterTrayDelegate() {};

  
  virtual void OnMessageCenterTrayChanged() = 0;

  
  
  virtual bool ShowPopups() = 0;

  
  virtual void HidePopups() = 0;

  
  
  virtual bool ShowMessageCenter() = 0;

  
  virtual void HideMessageCenter() = 0;

  
  virtual bool ShowNotifierSettings() = 0;

  
  
  virtual void DisplayFirstRunBalloon() {}

  virtual MessageCenterTray* GetMessageCenterTray() = 0;
};

}  

#endif  
