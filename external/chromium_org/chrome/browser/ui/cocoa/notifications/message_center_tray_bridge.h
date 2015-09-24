// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_MESSAGE_CENTER_TRAY_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_MESSAGE_CENTER_TRAY_BRIDGE_H_

#import <AppKit/AppKit.h>

#include "base/basictypes.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/message_center/message_center_tray_delegate.h"

@class MCPopupCollection;
@class MCStatusItemView;
@class MCTrayController;

namespace message_center {
class MessageCenter;
class MessageCenterTray;
}

class MessageCenterTrayBridge :
    public message_center::MessageCenterTrayDelegate {
 public:
  explicit MessageCenterTrayBridge(
      message_center::MessageCenter* message_center);
  virtual ~MessageCenterTrayBridge();

  
  virtual void OnMessageCenterTrayChanged() OVERRIDE;
  virtual bool ShowPopups() OVERRIDE;
  virtual void HidePopups() OVERRIDE;
  virtual bool ShowMessageCenter() OVERRIDE;
  virtual void HideMessageCenter() OVERRIDE;
  virtual bool ShowNotifierSettings() OVERRIDE;
  virtual message_center::MessageCenterTray* GetMessageCenterTray() OVERRIDE;

  message_center::MessageCenter* message_center() { return message_center_; }

 private:
  friend class MessageCenterTrayBridgeTest;

  
  void UpdateStatusItem();

  
  void OpenTrayWindow();

  
  message_center::MessageCenter* message_center_;

  
  scoped_ptr<message_center::MessageCenterTray> tray_;

  
  base::scoped_nsobject<MCTrayController> tray_controller_;

  
  base::scoped_nsobject<MCStatusItemView> status_item_view_;

  
  base::scoped_nsobject<MCPopupCollection> popup_collection_;

  
  base::WeakPtrFactory<MessageCenterTrayBridge> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessageCenterTrayBridge);
};

#endif  
