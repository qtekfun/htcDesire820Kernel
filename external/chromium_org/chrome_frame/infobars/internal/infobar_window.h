// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INTERNAL_INFOBAR_WINDOW_H_
#define CHROME_FRAME_INFOBARS_INTERNAL_INFOBAR_WINDOW_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"

#include "chrome_frame/infobars/infobar_content.h"
#include "chrome_frame/infobars/infobar_manager.h"

struct FunctionStub;

class InfobarWindow {
 public:
  
  class Host {
   public:
    virtual ~Host() {}

    
    
    
    virtual HWND GetContainerWindow() = 0;

    
    
    
    virtual void UpdateLayout() = 0;
  };  

  explicit InfobarWindow(InfobarType type);
  ~InfobarWindow();

  void SetHost(Host* host);

  
  
  
  
  
  
  bool Show(InfobarContent* content);

  
  void Hide();

  
  
  
  void ReserveSpace(RECT* rect);

 private:
  
  class FrameImpl : public InfobarContent::Frame {
   public:
    explicit FrameImpl(InfobarWindow* infobar_window);

    
    virtual HWND GetFrameWindow();
    virtual void CloseInfobar();

   private:
    InfobarWindow* infobar_window_;
    DISALLOW_COPY_AND_ASSIGN(FrameImpl);
  };  

  
  
  void StartSlidingTowards(int height);

  
  
  int CalculateHeight();

  
  bool StartTimer();
  bool StopTimer();

  scoped_ptr<InfobarContent> content_;
  Host* host_;
  FrameImpl frame_impl_;
  InfobarType type_;
  int current_width_;
  int current_height_;

  
  
  base::Time slide_start_;  
                            
  int initial_height_;      
  int target_height_;       

  
  int timer_id_;
  FunctionStub* timer_stub_;

  DISALLOW_COPY_AND_ASSIGN(InfobarWindow);
};  

#endif  
