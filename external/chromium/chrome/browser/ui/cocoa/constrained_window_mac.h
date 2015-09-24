// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "content/browser/tab_contents/constrained_window.h"

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_nsobject.h"

@class BrowserWindowController;
@class GTMWindowSheetController;
@class NSView;
@class NSWindow;
class TabContents;

class ConstrainedWindowMacDelegate {
 public:
  ConstrainedWindowMacDelegate() : is_sheet_open_(false) {}
  virtual ~ConstrainedWindowMacDelegate() {}

  
  
  
  virtual void DeleteDelegate() = 0;

  
  
  virtual void RunSheet(GTMWindowSheetController* sheetController,
                        NSView* view) = 0;
 protected:
  
  bool is_sheet_open() { return is_sheet_open_; }

 private:
  bool is_sheet_open_;
  void set_sheet_open(bool is_open) { is_sheet_open_ = is_open; }
  friend class ConstrainedWindowMac;
};

class ConstrainedWindowMacDelegateSystemSheet
    : public ConstrainedWindowMacDelegate {
 public:
  ConstrainedWindowMacDelegateSystemSheet(id delegate, SEL didEndSelector);
  virtual ~ConstrainedWindowMacDelegateSystemSheet();

 protected:
  void set_sheet(id sheet);
  id sheet() { return systemSheet_; }

  
  
  
  
  
  
  
  
  virtual NSArray* GetSheetParameters(id delegate, SEL didEndSelector);

 private:
  virtual void RunSheet(GTMWindowSheetController* sheetController,
                        NSView* view);
  scoped_nsobject<id> systemSheet_;
  scoped_nsobject<id> delegate_;
  SEL didEndSelector_;
};

class ConstrainedWindowMacDelegateCustomSheet
    : public ConstrainedWindowMacDelegate {
 public:
  ConstrainedWindowMacDelegateCustomSheet();
  ConstrainedWindowMacDelegateCustomSheet(id delegate, SEL didEndSelector);
  ~ConstrainedWindowMacDelegateCustomSheet();

 protected:
  
  void init(NSWindow* sheet, id delegate, SEL didEndSelector);
  void set_sheet(NSWindow* sheet);
  NSWindow* sheet() { return customSheet_; }

 private:
  virtual void RunSheet(GTMWindowSheetController* sheetController,
                        NSView* view);
  scoped_nsobject<NSWindow> customSheet_;
  scoped_nsobject<id> delegate_;
  SEL didEndSelector_;
};

class ConstrainedWindowMac : public ConstrainedWindow {
 public:
  virtual ~ConstrainedWindowMac();

  
  virtual void ShowConstrainedWindow();
  virtual void CloseConstrainedWindow();

  
  TabContents* owner() const { return owner_; }

  
  ConstrainedWindowMacDelegate* delegate() { return delegate_; }

  
  void Realize(BrowserWindowController* controller);

 private:
  friend class ConstrainedWindow;

  ConstrainedWindowMac(TabContents* owner,
                       ConstrainedWindowMacDelegate* delegate);

  
  TabContents* owner_;

  
  ConstrainedWindowMacDelegate* delegate_;

  
  BrowserWindowController* controller_;

  
  bool should_be_visible_;

  DISALLOW_COPY_AND_ASSIGN(ConstrainedWindowMac);
};

#endif  

