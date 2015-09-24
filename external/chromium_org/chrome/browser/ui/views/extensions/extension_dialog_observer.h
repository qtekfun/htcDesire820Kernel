// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_DIALOG_OBSERVER_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_DIALOG_OBSERVER_H_

class ExtensionDialog;

class ExtensionDialogObserver {
 public:
  ExtensionDialogObserver();
  virtual ~ExtensionDialogObserver();

  
  
  
  virtual void ExtensionDialogClosing(ExtensionDialog* popup) = 0;
  
  
  virtual void ExtensionTerminated(ExtensionDialog* popup) = 0;
};

#endif  
