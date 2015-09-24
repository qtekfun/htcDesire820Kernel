// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_H_
#pragma once

#include <string>

#include "base/basictypes.h"

class Browser;
class NavigationController;

class ToolbarModel {
 public:
  
  
  
  enum SecurityLevel {
    NONE = 0,          
    EV_SECURE,         
    SECURE,            
    SECURITY_WARNING,  
                       
    SECURITY_ERROR,    
    NUM_SECURITY_LEVELS,
  };

  explicit ToolbarModel(Browser* browser);
  ~ToolbarModel();

  
  std::wstring GetText() const;

  
  SecurityLevel GetSecurityLevel() const;

  
  
  
  int GetIcon() const;

  
  
  std::wstring GetEVCertName() const;

  
  
  void set_input_in_progress(bool value) { input_in_progress_ = value; }
  bool input_in_progress() const { return input_in_progress_; }

 private:
  
  
  
  NavigationController* GetNavigationController() const;

  Browser* browser_;

  
  bool input_in_progress_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ToolbarModel);
};

#endif  
