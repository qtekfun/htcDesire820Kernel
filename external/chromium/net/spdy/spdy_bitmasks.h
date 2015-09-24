// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_BITMASKS_H_
#define NET_SPDY_SPDY_BITMASKS_H_
#pragma once

namespace spdy {

const unsigned int kStreamIdMask = 0x7fffffff;

const unsigned int kControlFlagMask = 0x8000;

const unsigned int kPriorityMask = 0xc0;

const unsigned int kLengthMask = 0xffffff;

const unsigned int kSettingsIdMask = 0xffffff;

const int kDataFlagsMask = 0x03;

const int kControlFlagsMask = 0x03;

}  

#endif  
