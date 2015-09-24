// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_NEW_WINDOW_DELEGATE_H_
#define ASH_NEW_WINDOW_DELEGATE_H_

namespace ash {

class NewWindowDelegate {
 public:
  virtual ~NewWindowDelegate() {}

  
  virtual void NewTab() = 0;

  
  virtual void NewWindow(bool incognito) = 0;

  
  virtual void OpenFileManager() = 0;

  
  virtual void OpenCrosh() = 0;

  
  virtual void RestoreTab() = 0;

  
  virtual void ShowKeyboardOverlay() = 0;

  
  virtual void ShowTaskManager() = 0;

  
  virtual void OpenFeedbackPage() = 0;
};

}  

#endif  
