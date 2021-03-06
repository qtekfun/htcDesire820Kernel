// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_CROS_LIBRARY_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_CROS_CROS_LIBRARY_LOADER_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/cros/cros_library.h"

namespace chromeos {

class LibraryLoader {
 public:
  virtual ~LibraryLoader() {}
  virtual bool Load(std::string* load_error_string) = 0;
};

class CrosLibraryLoader : public LibraryLoader {
 public:
  CrosLibraryLoader() {}

  
  virtual bool Load(std::string* load_error_string);
};

}  

#endif  

