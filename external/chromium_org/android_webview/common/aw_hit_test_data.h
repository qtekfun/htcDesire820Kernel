// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_COMMON_AW_HIT_TEST_DATA_H_
#define ANDROID_WEBVIEW_COMMON_AW_HIT_TEST_DATA_H_

#include "base/strings/string16.h"
#include "url/gurl.h"

namespace android_webview {

struct AwHitTestData {

  
  
  enum Type {
    
    
    
    
    
    
    
    UNKNOWN_TYPE = 0,

    
    
    
    
    PHONE_TYPE = 2,
    GEO_TYPE = 3,
    EMAIL_TYPE = 4,

    
    
    
    IMAGE_TYPE = 5,

    
    
    
    
    
    
    
    
    
    
    
    SRC_LINK_TYPE = 7,

    
    
    
    
    SRC_IMAGE_LINK_TYPE = 8,

    
    EDIT_TEXT_TYPE = 9,
  };

  
  
  int type;  
  std::string extra_data_for_type;
  string16 href;
  string16 anchor_text;
  GURL img_src;

  AwHitTestData();
  ~AwHitTestData();
};

}  

#endif  
