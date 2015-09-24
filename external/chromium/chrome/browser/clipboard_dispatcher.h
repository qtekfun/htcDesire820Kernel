// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CLIPBOARD_DISPATCHER_H_
#define CHROME_BROWSER_CLIPBOARD_DISPATCHER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "ui/base/clipboard/clipboard.h"

class ClipboardDispatcher {
 public:
  static bool ReadAvailableTypes(ui::Clipboard::Buffer buffer,
                                 std::vector<string16>* types,
                                 bool* contains_filenames);
  static bool ReadData(ui::Clipboard::Buffer buffer, const string16& type,
                       string16* data, string16* metadata);
  static bool ReadFilenames(ui::Clipboard::Buffer buffer,
                            std::vector<string16>* filenames);

 private:
  
  ClipboardDispatcher();

  DISALLOW_COPY_AND_ASSIGN(ClipboardDispatcher);
};

#endif  
