// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_MAGNIFIER_MAGNIFIER_CONSTANTS_H_
#define ASH_MAGNIFIER_MAGNIFIER_CONSTANTS_H_

namespace ash {

enum MagnifierType {
  MAGNIFIER_FULL    = 1,
  MAGNIFIER_PARTIAL = 2,
};

const int kMaxMagnifierType = 2;

const MagnifierType kDefaultMagnifierType = MAGNIFIER_FULL;

const float kMagnificationScaleFactor = 1.18920712f;

}  

#endif  
