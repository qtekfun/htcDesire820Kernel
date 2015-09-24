// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_VIEW_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_VIEW_BRIDGE_H_
#pragma once

#include "chrome/browser/notifications/balloon.h"

@class BalloonController;
class BalloonHost;
namespace gfx {
class Size;
}

class BalloonViewBridge : public BalloonView {
 public:
  BalloonViewBridge();
  ~BalloonViewBridge();

  
  virtual void Show(Balloon* balloon);
  virtual void Update();
  virtual void RepositionToBalloon();
  virtual void Close(bool by_user);
  virtual gfx::Size GetSize() const;
  virtual BalloonHost* GetHost() const;

 private:
  
  
  BalloonController* controller_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewBridge);
};

#endif  
