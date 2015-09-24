// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WINDOW_CONTROLLER_LIST_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_WINDOW_CONTROLLER_LIST_OBSERVER_H_

namespace extensions {

class WindowController;

class WindowControllerListObserver {
  public:
  
  virtual void OnWindowControllerAdded(WindowController* window_controller) {}

  
  virtual void OnWindowControllerRemoved(WindowController* window_controller) {}

 protected:
  virtual ~WindowControllerListObserver() {}
};

}  

#endif  
