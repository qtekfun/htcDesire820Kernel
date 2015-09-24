// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMMAND_OBSERVER_H_
#define CHROME_BROWSER_COMMAND_OBSERVER_H_

class CommandObserver {
 public:
  
  
  virtual void EnabledStateChangedForCommand(int id, bool enabled) = 0;

 protected:
  virtual ~CommandObserver() {}
};

#endif  
