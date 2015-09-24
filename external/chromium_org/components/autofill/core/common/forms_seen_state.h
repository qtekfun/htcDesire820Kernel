// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_COMMON_FORMS_SEEN_PARAM_H_
#define COMPONENTS_AUTOFILL_COMMON_FORMS_SEEN_PARAM_H_

namespace autofill {

enum FormsSeenState {
  
  NO_SPECIAL_FORMS_SEEN = 0,
  
  PARTIAL_FORMS_SEEN = 1,
  
  DYNAMIC_FORMS_SEEN = 2,
  
  FORMS_SEEN_STATE_NUM_STATES = 3,
};

}  

#endif  
