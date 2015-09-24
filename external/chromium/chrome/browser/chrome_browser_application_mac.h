// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_APPLICATION_MAC_H_
#define CHROME_BROWSER_CHROME_BROWSER_APPLICATION_MAC_H_
#pragma once

#ifdef __OBJC__

#import "content/common/chrome_application_mac.h"

@interface BrowserCrApplication : CrApplication
- (void)cancelTerminate:(id)sender;
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
