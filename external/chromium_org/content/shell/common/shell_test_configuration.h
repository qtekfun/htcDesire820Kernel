// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_COMMON_SHELL_TEST_CONFIGURATION_H_
#define CONTENT_SHELL_COMMON_SHELL_TEST_CONFIGURATION_H_

#include <string>

#include "base/files/file_path.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

namespace content {

struct ShellTestConfiguration {
  ShellTestConfiguration();
  ~ShellTestConfiguration();

  
  base::FilePath current_working_directory;

  
  base::FilePath temp_path;

  
  GURL test_url;

  
  bool enable_pixel_dumping;

  
  bool allow_external_pages;

  
  std::string expected_pixel_hash;

  
  gfx::Size initial_size;
};

}  

#endif  
