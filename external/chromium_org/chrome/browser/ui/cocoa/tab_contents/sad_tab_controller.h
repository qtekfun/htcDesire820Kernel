// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_SAD_TAB_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/sad_tab.h"

#import <Cocoa/Cocoa.h>

@class SadTabController;

namespace chrome {

class SadTabCocoa : public SadTab {
 public:
  explicit SadTabCocoa(content::WebContents* web_contents);

  virtual ~SadTabCocoa();

 private:
  
  virtual void Show() OVERRIDE;
  virtual void Close() OVERRIDE;

  base::scoped_nsobject<SadTabController> sad_tab_controller_;

  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(SadTabCocoa);
};

}  

@interface SadTabController : NSViewController {
 @private
  content::WebContents* webContents_;  
}

- (id)initWithWebContents:(content::WebContents*)webContents;

- (IBAction)openLearnMoreAboutCrashLink:(id)sender;

- (content::WebContents*)webContents;

@end

#endif  
