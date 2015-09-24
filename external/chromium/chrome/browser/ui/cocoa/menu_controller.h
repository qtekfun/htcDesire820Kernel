// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_MENU_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"

namespace ui {
class MenuModel;
}

@interface MenuController : NSObject<NSMenuDelegate> {
 @protected
  ui::MenuModel* model_;  
  scoped_nsobject<NSMenu> menu_;
  BOOL useWithPopUpButtonCell_;  
}

@property(nonatomic, assign) ui::MenuModel* model;
@property(nonatomic) BOOL useWithPopUpButtonCell;

- (id)init;

- (id)initWithModel:(ui::MenuModel*)model
    useWithPopUpButtonCell:(BOOL)useWithCell;

- (NSMenu*)menu;

@end

@interface MenuController (PrivateExposedForTesting)
- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item;
@end

@interface MenuController (Protected)
- (void)addItemToMenu:(NSMenu*)menu
              atIndex:(NSInteger)index
            fromModel:(ui::MenuModel*)model
           modelIndex:(int)modelIndex;
@end

#endif  
