// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_ATTRITION_EXPERIMENTS_H_
#define CHROME_COMMON_ATTRITION_EXPERIMENTS_H_

#include "grit/chromium_strings.h"

namespace attrition_experiments {

enum Experiment {
  kEnUs1 = IDS_TRY_TOAST_HEADING,
  kEnUs2 = IDS_TRY_TOAST_HEADING2,
  kEnUs3 = IDS_TRY_TOAST_HEADING3,
  kEnUs4 = IDS_TRY_TOAST_HEADING4,
  kSkype1 = IDS_TRY_TOAST_HEADING_SKYPE,
};

const wchar_t kSkypeBrandCode[] = L"SKPC,SKPG,SKPH,SKPI,SKPL,SKPM,SKPN";

}  

#endif  
