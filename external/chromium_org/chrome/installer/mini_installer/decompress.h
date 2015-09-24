// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_MINI_INSTALLER_DECOMPRESS_H_
#define CHROME_INSTALLER_MINI_INSTALLER_DECOMPRESS_H_

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

namespace mini_installer {

bool Expand(const wchar_t* source, const wchar_t* destination);

}  

#endif  
