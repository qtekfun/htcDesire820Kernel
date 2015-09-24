// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SAFE_STRERROR_POSIX_H_
#define BASE_SAFE_STRERROR_POSIX_H_
#pragma once

#include <string>


void safe_strerror_r(int err, char *buf, size_t len);

std::string safe_strerror(int err);

#endif  
