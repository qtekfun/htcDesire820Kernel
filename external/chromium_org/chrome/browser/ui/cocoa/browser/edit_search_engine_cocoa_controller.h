// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_EDIT_SEARCH_ENGINE_COCOA_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_EDIT_SEARCH_ENGINE_COCOA_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/search_engines/edit_search_engine_controller.h"

class TemplateURL;

@interface EditSearchEngineCocoaController :
    NSWindowController<NSWindowDelegate> {
  IBOutlet NSTextField* nameField_;
  IBOutlet NSTextField* keywordField_;
  IBOutlet NSTextField* urlField_;
  IBOutlet NSImageView* nameImage_;
  IBOutlet NSImageView* keywordImage_;
  IBOutlet NSImageView* urlImage_;
  IBOutlet NSButton* doneButton_;
  IBOutlet NSTextField* urlDescriptionField_;
  IBOutlet NSView* labelContainer_;
  IBOutlet NSBox* fieldAndImageContainer_;

  Profile* profile_;  
  TemplateURL* templateURL_;  
  scoped_ptr<EditSearchEngineController> controller_;
}

- (id)initWithProfile:(Profile*)profile
             delegate:(EditSearchEngineControllerDelegate*)delegate
          templateURL:(TemplateURL*)url;

- (IBAction)cancel:(id)sender;
- (IBAction)save:(id)sender;

@end

@interface EditSearchEngineCocoaController (ExposedForTesting)
- (BOOL)validateFields;
@end

#endif  
