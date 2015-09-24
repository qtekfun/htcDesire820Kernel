// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APPS_PAGINATION_MODEL_OBSERVER_H_
#define UI_APP_LIST_COCOA_APPS_PAGINATION_MODEL_OBSERVER_H_

@protocol AppsPaginationModelObserver

- (void)totalPagesChanged;

- (void)selectedPageChanged:(int)newSelected;

- (void)pageVisibilityChanged;

- (NSInteger)pagerSegmentAtLocation:(NSPoint)locationInWindow;

@end

#endif  
