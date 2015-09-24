// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_STATUS_BUBBLE_MAC_H_
#define CHROME_BROWSER_UI_COCOA_STATUS_BUBBLE_MAC_H_
#pragma once

#include <string>

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/ui/status_bubble.h"
#include "googleurl/src/gurl.h"

class GURL;
class StatusBubbleMacTest;

class StatusBubbleMac : public StatusBubble {
 public:
  
  
  enum StatusBubbleState {
    kBubbleHidden,         
    kBubbleShowingTimer,   
    kBubbleShowingFadeIn,  
    kBubbleShown,          
    kBubbleHidingTimer,    
    kBubbleHidingFadeOut   
  };

  StatusBubbleMac(NSWindow* parent, id delegate);
  virtual ~StatusBubbleMac();

  
  virtual void SetStatus(const string16& status);
  virtual void SetURL(const GURL& url, const string16& languages);
  virtual void Hide();
  virtual void MouseMoved(const gfx::Point& location, bool left_content);
  virtual void UpdateDownloadShelfVisibility(bool visible);

  
  
  
  void UpdateSizeAndPosition();

  
  
  void SwitchParentWindow(NSWindow* parent);

  
  
  
  void AnimationDidStop(CAAnimation* animation, bool finished);

  
  void ExpandBubble();

 private:
  friend class StatusBubbleMacTest;

  
  
  void SetState(StatusBubbleState state);

  
  void SetText(const string16& text, bool is_url);

  
  
  void Create();

  
  
  void Attach();

  
  void Detach();

  
  
  bool is_attached() { return [window_ parentWindow] != nil; }

  
  
  
  
  
  
  void Fade(bool show);

  
  
  
  
  
  void StartTimer(int64 time_ms);
  void CancelTimer();
  void TimerFired();

  
  
  
  void StartShowing();
  void StartHiding();

  
  void CancelExpandTimer();

  
  ScopedRunnableMethodFactory<StatusBubbleMac> timer_factory_;

  
  ScopedRunnableMethodFactory<StatusBubbleMac> expand_timer_factory_;

  
  
  NSRect CalculateWindowFrame(bool expanded_width);

  
  NSWindow* parent_;  

  
  id delegate_;  

  
  NSWindow* window_;

  
  NSString* status_text_;

  
  NSString* url_text_;

  
  
  StatusBubbleState state_;

  
  
  
  bool immediate_;

  
  
  
  bool is_expanded_;

  
  GURL url_;

  
  
  string16 languages_;

  DISALLOW_COPY_AND_ASSIGN(StatusBubbleMac);
};

@interface NSObject(StatusBubbleDelegate)
- (NSRect)statusBubbleBaseFrame;

- (void)statusBubbleWillEnterState:(StatusBubbleMac::StatusBubbleState)state;
@end

#endif  
