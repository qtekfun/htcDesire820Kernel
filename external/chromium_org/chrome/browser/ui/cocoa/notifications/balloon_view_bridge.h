// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_VIEW_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_NOTIFICATIONS_BALLOON_VIEW_BRIDGE_H_

#include "chrome/browser/notifications/balloon.h"

@class BalloonController;
class BalloonHost;
namespace gfx {
class Size;
}

class BalloonViewBridge : public BalloonView {
 public:
  BalloonViewBridge();
  virtual ~BalloonViewBridge();

  
  virtual void Show(Balloon* balloon) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual void RepositionToBalloon() OVERRIDE;
  virtual void Close(bool by_user) OVERRIDE;
  virtual gfx::Size GetSize() const OVERRIDE;
  virtual BalloonHost* GetHost() const OVERRIDE;

 private:
  
  
  BalloonController* controller_;

  DISALLOW_COPY_AND_ASSIGN(BalloonViewBridge);
};

#endif  
