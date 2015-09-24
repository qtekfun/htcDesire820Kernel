// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_SHORTCUT_WIN_H_
#define BASE_TEST_TEST_SHORTCUT_WIN_H_

#include "base/files/file_path.h"
#include "base/win/shortcut.h"


namespace base {
namespace win {

void ValidateShortcut(const FilePath& shortcut_path,
                      const ShortcutProperties& properties);

}  
}  

#endif  
