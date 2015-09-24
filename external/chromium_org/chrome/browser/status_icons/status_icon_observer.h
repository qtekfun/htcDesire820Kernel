// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_OBSERVER_H_
#define CHROME_BROWSER_STATUS_ICONS_STATUS_ICON_OBSERVER_H_

class StatusIconObserver {
 public:
  
  
  
  
  
  
  
  virtual void OnStatusIconClicked() = 0;

#if defined(OS_WIN)
  
  
  
  virtual void OnBalloonClicked() {}
#endif

 protected:
  virtual ~StatusIconObserver() {}
};

#endif  
