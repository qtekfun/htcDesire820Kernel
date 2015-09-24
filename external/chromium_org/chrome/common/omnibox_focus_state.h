// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_OMNIBOX_FOCUS_STATE_H_
#define CHROME_COMMON_OMNIBOX_FOCUS_STATE_H_

enum OmniboxFocusState {
  
  OMNIBOX_FOCUS_NONE,

  
  OMNIBOX_FOCUS_VISIBLE,

  
  
  
  
  OMNIBOX_FOCUS_INVISIBLE,
};

enum OmniboxFocusChangeReason {
  
  
  
  OMNIBOX_FOCUS_CHANGE_EXPLICIT,

  
  OMNIBOX_FOCUS_CHANGE_TAB_SWITCH,

  
  
  OMNIBOX_FOCUS_CHANGE_TYPING,
};

#endif  
