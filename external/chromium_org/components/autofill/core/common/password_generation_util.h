// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_GENERATION_UTIL_H_
#define COMPONENTS_AUTOFILL_CORE_COMMON_PASSWORD_GENERATION_UTIL_H_

namespace autofill {
namespace password_generation {

enum PasswordGenerationEvent {
  
  NO_SIGN_UP_DETECTED,

  
  SIGN_UP_DETECTED,

  
  ICON_SHOWN,

  
  BUBBLE_SHOWN,

  
  EVENT_ENUM_COUNT
};

struct PasswordGenerationActions {
  
  bool learn_more_visited;

  
  bool password_accepted;

  
  bool password_edited;

  
  bool password_regenerated;

  PasswordGenerationActions();
  ~PasswordGenerationActions();
};

void LogUserActions(PasswordGenerationActions actions);

void LogPasswordGenerationEvent(PasswordGenerationEvent event);

enum UserAction {
  
  
  IGNORE_FEATURE,

  
  
  
  LEARN_MORE,

  
  
  ACCEPT_ORIGINAL_PASSWORD,

  
  ACCEPT_AFTER_EDITING,

  
  ACTION_ENUM_COUNT
};

bool IsPasswordGenerationEnabled();

}  
}  

#endif  
