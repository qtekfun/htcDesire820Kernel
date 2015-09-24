// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_USER_ACTION_CLIENT_H_
#define UI_AURA_CLIENT_USER_ACTION_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;
namespace client {

class AURA_EXPORT UserActionClient {
 public:
  enum Command {
    BACK = 0,
    FORWARD,
  };

  
  virtual bool OnUserAction(Command command) = 0;

  virtual ~UserActionClient() {}
};

AURA_EXPORT void SetUserActionClient(Window* root_window,
                                     UserActionClient* client);
AURA_EXPORT UserActionClient* GetUserActionClient(Window* root_window);

}  
}  

#endif  
