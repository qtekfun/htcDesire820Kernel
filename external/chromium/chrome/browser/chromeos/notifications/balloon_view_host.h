// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_VIEW_HOST_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_BALLOON_VIEW_HOST_H_
#pragma once

#include "chrome/browser/ui/views/notifications/balloon_view_host.h"

#include <map>
#include <string>

#include "base/callback.h"
#include "ui/gfx/native_widget_types.h"

class ListValue;
class GURL;

namespace chromeos {

typedef Callback1<const ListValue*>::Type MessageCallback;

class BalloonViewHost : public ::BalloonViewHost {
 public:
  explicit BalloonViewHost(Balloon* balloon) : ::BalloonViewHost(balloon) {}
  virtual ~BalloonViewHost();

  
  
  
  
  bool AddWebUIMessageCallback(const std::string& message,
                               MessageCallback* callback);

  
  virtual void ProcessWebUIMessage(
      const ExtensionHostMsg_DomMessage_Params& params);

 private:
  
  typedef std::map<std::string, MessageCallback*> MessageCallbackMap;
  MessageCallbackMap message_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewHost);
};

}  

#endif  
