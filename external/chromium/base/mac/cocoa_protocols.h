// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_COCOA_PROTOCOLS_MAC_H_
#define BASE_COCOA_PROTOCOLS_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "third_party/GTM/GTMDefines.h"


#if !defined(MAC_OS_X_VERSION_10_6) || \
    MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_6

#define DEFINE_EMPTY_PROTOCOL(p) \
@protocol p \
@end

DEFINE_EMPTY_PROTOCOL(NSAlertDelegate)
DEFINE_EMPTY_PROTOCOL(NSApplicationDelegate)
DEFINE_EMPTY_PROTOCOL(NSControlTextEditingDelegate)
DEFINE_EMPTY_PROTOCOL(NSMatrixDelegate)
DEFINE_EMPTY_PROTOCOL(NSMenuDelegate)
DEFINE_EMPTY_PROTOCOL(NSOpenSavePanelDelegate)
DEFINE_EMPTY_PROTOCOL(NSOutlineViewDataSource)
DEFINE_EMPTY_PROTOCOL(NSOutlineViewDelegate)
DEFINE_EMPTY_PROTOCOL(NSTableViewDataSource)
DEFINE_EMPTY_PROTOCOL(NSTableViewDelegate)
DEFINE_EMPTY_PROTOCOL(NSTextFieldDelegate)
DEFINE_EMPTY_PROTOCOL(NSTextViewDelegate)
DEFINE_EMPTY_PROTOCOL(NSWindowDelegate)

#undef DEFINE_EMPTY_PROTOCOL

#endif

#endif  
