// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_LISTENER_H_
#define UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_LISTENER_H_

namespace views {

class TabbedPaneListener {
 public:
  
  virtual void TabSelectedAt(int index) = 0;

 protected:
  virtual ~TabbedPaneListener() {}
};

}  

#endif  
