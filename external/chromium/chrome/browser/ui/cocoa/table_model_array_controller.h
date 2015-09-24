// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABLE_MODEL_ARRAY_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABLE_MODEL_ARRAY_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/models/table_model_observer.h"

class RemoveRowsObserverBridge;
class RemoveRowsTableModel;
@class TableModelArrayController;

@interface TableModelArrayController : NSArrayController<NSTableViewDelegate> {
 @private
  RemoveRowsTableModel* model_; 
  scoped_ptr<RemoveRowsObserverBridge> tableObserver_;
  scoped_nsobject<NSDictionary> columns_;
  scoped_nsobject<NSString> groupTitle_;
}

- (void)bindToTableModel:(RemoveRowsTableModel*)model
             withColumns:(NSDictionary*)columns
        groupTitleColumn:(NSString*)groupTitleColumn;

- (IBAction)removeAll:(id)sender;

@end

#endif  

