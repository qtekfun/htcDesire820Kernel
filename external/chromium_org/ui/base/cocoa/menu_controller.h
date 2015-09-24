// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_MENU_CONTROLLER_H_
#define UI_BASE_COCOA_MENU_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {
class MenuModel;
}

UI_EXPORT
@interface MenuController : NSObject<NSMenuDelegate> {
 @protected
  ui::MenuModel* model_;  
  base::scoped_nsobject<NSMenu> menu_;
  BOOL useWithPopUpButtonCell_;  
  BOOL isMenuOpen_;
}

@property(nonatomic, assign) ui::MenuModel* model;
@property(nonatomic) BOOL useWithPopUpButtonCell;

+ (string16)elideMenuTitle:(const string16&)title
                   toWidth:(int)width;

- (id)init;

- (id)initWithModel:(ui::MenuModel*)model
    useWithPopUpButtonCell:(BOOL)useWithCell;

- (void)cancel;

- (NSMenu*)menu;

- (BOOL)isMenuOpen;

- (void)menuWillOpen:(NSMenu*)menu;
- (void)menuDidClose:(NSMenu*)menu;

@end

@interface MenuController (PrivateExposedForTesting)
- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item;
@end

@interface MenuController (Protected)
- (void)addItemToMenu:(NSMenu*)menu
              atIndex:(NSInteger)index
            fromModel:(ui::MenuModel*)model;
- (NSMenu*)menuFromModel:(ui::MenuModel*)model;
- (int)maxWidthForMenuModel:(ui::MenuModel*)model
                 modelIndex:(int)modelIndex;
@end

#endif  
