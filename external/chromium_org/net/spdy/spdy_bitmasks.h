// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_BITMASKS_H_
#define NET_SPDY_SPDY_BITMASKS_H_

namespace net {

const unsigned int kStreamIdMask = 0x7fffffff;

const unsigned int kControlFlagMask = 0x8000;

const unsigned int kLengthMask = 0xffffff;

const int kDataFlagsMask = 0x01;

const int kControlFlagsMask = 0x03;

}  

#endif  
