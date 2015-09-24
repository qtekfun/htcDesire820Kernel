// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_READY_MODE_INTERNAL_URL_LAUNCHER_H_
#define CHROME_FRAME_READY_MODE_INTERNAL_URL_LAUNCHER_H_

class UrlLauncher {
 public:
  virtual ~UrlLauncher() {}

  
  virtual void LaunchUrl(const std::wstring& url) = 0;
};  

#endif  
