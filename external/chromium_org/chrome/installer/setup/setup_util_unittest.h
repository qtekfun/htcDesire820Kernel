// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_SETUP_SETUP_UTIL_UNITTEST_H_
#define CHROME_INSTALLER_SETUP_SETUP_UTIL_UNITTEST_H_

extern const char kAdjustProcessPriority[];

enum PriorityClassChangeResult {
  PCCR_UNKNOWN,
  PCCR_UNCHANGED,
  PCCR_CHANGED,
};

PriorityClassChangeResult DoProcessPriorityAdjustment();

#endif  
