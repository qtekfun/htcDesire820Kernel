// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_STATUS_BUBBLE_MAC_H_
#define CHROME_BROWSER_UI_COCOA_STATUS_BUBBLE_MAC_H_

#include <string>

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/status_bubble.h"
#include "url/gurl.h"

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

  
  virtual void SetStatus(const base::string16& status) OVERRIDE;
  virtual void SetURL(const GURL& url, const std::string& languages) OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void MouseMoved(const gfx::Point& location,
                          bool left_content) OVERRIDE;
  virtual void UpdateDownloadShelfVisibility(bool visible) OVERRIDE;

  
  
  
  void UpdateSizeAndPosition();

  
  
  void SwitchParentWindow(NSWindow* parent);

  
  
  
  void AnimationDidStop(CAAnimation* animation, bool finished);

  
  void ExpandBubble();

 protected:
  
  
  virtual gfx::Point GetMouseLocation();

 private:
  friend class StatusBubbleMacTest;

  
  
  void SetState(StatusBubbleState state);

  
  void SetText(const base::string16& text, bool is_url);

  
  
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

  
  
  void SetFrameAvoidingMouse(NSRect window_frame, const gfx::Point& mouse_pos);

  
  base::WeakPtrFactory<StatusBubbleMac> timer_factory_;

  
  base::WeakPtrFactory<StatusBubbleMac> expand_timer_factory_;

  
  
  NSRect CalculateWindowFrame(bool expanded_width);

  
  
  
  
  
  
  
  
  
  unsigned long OSDependentCornerFlags(NSRect window_frame);

  
  NSWindow* parent_;  

  
  id delegate_;  

  
  NSWindow* window_;

  
  NSString* status_text_;

  
  NSString* url_text_;

  
  
  StatusBubbleState state_;

  
  
  
  bool immediate_;

  
  
  
  bool is_expanded_;

  
  GURL url_;

  
  
  std::string languages_;

  DISALLOW_COPY_AND_ASSIGN(StatusBubbleMac);
};

@interface NSObject(StatusBubbleDelegate)
- (NSRect)statusBubbleBaseFrame;

- (void)statusBubbleWillEnterState:(StatusBubbleMac::StatusBubbleState)state;
@end

#endif  
