// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_FILE_CHOOSER_PARAMS_H_
#define CONTENT_PUBLIC_COMMON_FILE_CHOOSER_PARAMS_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

namespace content {

struct CONTENT_EXPORT FileChooserParams {
  FileChooserParams();
  ~FileChooserParams();

  enum Mode {
    
    Open,

    
    OpenMultiple,

    
    UploadFolder,

    
    
    Save,
  };

  Mode mode;

  
  
  base::string16 title;

  
  base::FilePath default_file_name;

  
  
  std::vector<base::string16> accept_types;

#if defined(OS_ANDROID)
  
  
  bool capture;
#endif
};

}  

#endif  
