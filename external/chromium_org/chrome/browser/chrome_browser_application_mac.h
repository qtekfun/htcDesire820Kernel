// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_APPLICATION_MAC_H_
#define CHROME_BROWSER_CHROME_BROWSER_APPLICATION_MAC_H_

#ifdef __OBJC__

#import <AppKit/AppKit.h>

#include <vector>

#import "base/mac/scoped_sending_event.h"
#import "base/message_loop/message_pump_mac.h"
#include "base/synchronization/lock.h"

@interface BrowserCrApplication : NSApplication<CrAppProtocol,
                                                CrAppControlProtocol> {
 @private
  BOOL handlingSendEvent_;
  BOOL cyclingWindows_;

  
  
  
  std::vector<NSWindow*> previousKeyWindows_;

  
  base::Lock previousKeyWindowsLock_;
}

- (void)cancelTerminate:(id)sender;

- (NSWindow*)previousKeyWindow;
- (BOOL)isCyclingWindows;
@end

namespace chrome_browser_application_mac {

extern const size_t kUnknownNSException;

size_t BinForException(NSException* exception);

void RecordExceptionWithUma(NSException* exception);

}  

#endif  

namespace chrome_browser_application_mac {

void RegisterBrowserCrApp();

void Terminate();

void CancelTerminate();

}  

#endif  
