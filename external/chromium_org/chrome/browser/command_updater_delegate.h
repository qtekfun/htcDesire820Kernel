// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMMAND_UPDATER_DELEGATE_H_
#define CHROME_BROWSER_COMMAND_UPDATER_DELEGATE_H_

#include "ui/base/window_open_disposition.h"

class CommandUpdaterDelegate {
 public:
  
  
  virtual void ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition) = 0;

 protected:
  virtual ~CommandUpdaterDelegate() {}
};

#endif  
