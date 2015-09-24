// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_COMMAND_OBSERVER_BRIDGE
#define CHROME_BROWSER_UI_COCOA_COMMAND_OBSERVER_BRIDGE

#import <Cocoa/Cocoa.h>

#include "base/compiler_specific.h"
#include "chrome/browser/command_observer.h"

@protocol CommandObserverProtocol;

class CommandUpdater;


class CommandObserverBridge : public CommandObserver {
 public:
  CommandObserverBridge(id<CommandObserverProtocol> observer,
                        CommandUpdater* commands);
  virtual ~CommandObserverBridge();

  
  void ObserveCommand(int command);

 protected:
  
  virtual void EnabledStateChangedForCommand(int command,
                                             bool enabled) OVERRIDE;

 private:
  id<CommandObserverProtocol> observer_;  
  CommandUpdater* commands_;  
};

@protocol CommandObserverProtocol
- (void)enabledStateChangedForCommand:(NSInteger)command enabled:(BOOL)enabled;
@end

#endif  
