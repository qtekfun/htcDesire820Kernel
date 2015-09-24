// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_NETWORK_TRAY_SMS_H
#define ASH_SYSTEM_CHROMEOS_NETWORK_TRAY_SMS_H

#include <string>

#include "ash/system/tray/system_tray_item.h"
#include "base/values.h"
#include "chromeos/network/network_sms_handler.h"

namespace ash {
namespace internal {

class TraySms : public SystemTrayItem,
                public chromeos::NetworkSmsHandler::Observer {
 public:
  explicit TraySms(SystemTray* system_tray);
  virtual ~TraySms();

  
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateDetailedView(user::LoginStatus status) OVERRIDE;
  virtual views::View* CreateNotificationView(
      user::LoginStatus status) OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;
  virtual void DestroyDetailedView() OVERRIDE;
  virtual void DestroyNotificationView() OVERRIDE;

  
  virtual void MessageReceived(const base::DictionaryValue& message) OVERRIDE;

 protected:
  class SmsDefaultView;
  class SmsDetailedView;
  class SmsMessageView;
  class SmsNotificationView;

  
  
  bool GetLatestMessage(size_t* index, std::string* number, std::string* text);

  
  void RemoveMessage(size_t index);

  
  
  void Update(bool notify);

  base::ListValue& messages() { return messages_; }

 private:
  SmsDefaultView* default_;
  SmsDetailedView* detailed_;
  SmsNotificationView* notification_;
  base::ListValue messages_;

  DISALLOW_COPY_AND_ASSIGN(TraySms);
};

}  
}  

#endif  
