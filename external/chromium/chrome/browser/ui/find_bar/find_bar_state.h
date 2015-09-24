// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_STATE_H_
#define CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_STATE_H_
#pragma once

#include "base/basictypes.h"
#include "base/string16.h"

class Profile;

class FindBarState {
 public:
  FindBarState() {}
  ~FindBarState() {}

  string16 last_prepopulate_text() const {
    return last_prepopulate_text_;
  }

  void set_last_prepopulate_text(const string16& text) {
    last_prepopulate_text_ = text;
  }

  
  
  
  static string16 GetLastPrepopulateText(Profile* profile);

 private:
  string16 last_prepopulate_text_;

  DISALLOW_COPY_AND_ASSIGN(FindBarState);
};

#endif  
