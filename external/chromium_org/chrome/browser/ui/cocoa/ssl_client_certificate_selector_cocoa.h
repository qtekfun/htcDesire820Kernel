// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_SSL_CLIENT_CERTIFICATE_SELECTOR_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_SSL_CLIENT_CERTIFICATE_SELECTOR_COCOA_H_

#import <Cocoa/Cocoa.h>
#include <vector>

#include "base/mac/scoped_cftyperef.h"
#include "base/mac/scoped_nsobject.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ssl/ssl_client_certificate_selector.h"
#import "chrome/browser/ui/cocoa/constrained_window/constrained_window_custom_sheet.h"
#import "chrome/browser/ui/cocoa/constrained_window/constrained_window_sheet_controller.h"

class ConstrainedWindowMac;
@class SFChooseIdentityPanel;
class SSLClientAuthObserverCocoaBridge;

@interface SSLClientCertificateSelectorCocoa
    : NSObject<ConstrainedWindowSheet> {
 @private
  
  base::ScopedCFTypeRef<CFMutableArrayRef> identities_;
  
  std::vector<scoped_refptr<net::X509Certificate> > certificates_;
  
  scoped_ptr<SSLClientAuthObserverCocoaBridge> observer_;
  base::scoped_nsobject<SFChooseIdentityPanel> panel_;
  scoped_ptr<ConstrainedWindowMac> constrainedWindow_;
  base::scoped_nsobject<NSWindow> overlayWindow_;
  BOOL closePending_;
  
  NSRect oldSheetFrame_;
  
  BOOL oldResizesSubviews_;
}

@property (readonly, nonatomic) SFChooseIdentityPanel* panel;

- (id)initWithNetworkSession:(const net::HttpNetworkSession*)networkSession
             certRequestInfo:(net::SSLCertRequestInfo*)certRequestInfo
                    callback:(const chrome::SelectCertificateCallback&)callback;
- (void)displayForWebContents:(content::WebContents*)webContents;
- (void)closeWebContentsModalDialog;

- (NSWindow*)overlayWindow;

@end

#endif  
