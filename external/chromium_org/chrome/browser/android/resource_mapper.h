// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_RESOURCE_MAPPER_H_
#define CHROME_BROWSER_ANDROID_RESOURCE_MAPPER_H_

#include <map>

class ResourceMapper {
 public:
  
  
  static const int kMissingId;

  
  
  static int MapFromChromiumId(int resource_id);

 private:
  
  
  static void ConstructMap();
};

#endif  
