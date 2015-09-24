// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INFOBAR_CONTENT_H_
#define CHROME_FRAME_INFOBARS_INFOBAR_CONTENT_H_

#include <windows.h>

class InfobarContent {
 public:
  
  
  class Frame {
   public:
    virtual ~Frame() {}

    
    virtual HWND GetFrameWindow() = 0;

    
    virtual void CloseInfobar() = 0;
  };  

  virtual ~InfobarContent() {}

  
  
  
  
  virtual bool InstallInFrame(Frame* frame) = 0;

  
  
  virtual void SetDimensions(const RECT& dimensions) = 0;

  
  
  
  virtual size_t GetDesiredSize(size_t width, size_t height) = 0;
};  

#endif  
