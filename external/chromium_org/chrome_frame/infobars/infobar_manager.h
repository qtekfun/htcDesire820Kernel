// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INFOBAR_MANAGER_H_
#define CHROME_FRAME_INFOBARS_INFOBAR_MANAGER_H_

#include <windows.h>

class InfobarContent;

enum InfobarType {
  FIRST_INFOBAR_TYPE = 0,
  TOP_INFOBAR = 0,          
  BOTTOM_INFOBAR = 1,       
  END_OF_INFOBAR_TYPE = 2
};

class InfobarManager {
 public:
  
  
  
  
  
  
  
  static InfobarManager* Get(HWND tab_window);

  virtual ~InfobarManager();

  
  
  
  
  
  
  virtual bool Show(InfobarContent* content, InfobarType type) = 0;

  
  virtual void Hide(InfobarType type) = 0;

  
  virtual void HideAll() = 0;
};  

#endif  
