// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_WEBRTC_LOG_UPLOAD_LIST_H_
#define CHROME_BROWSER_MEDIA_WEBRTC_LOG_UPLOAD_LIST_H_

#include "chrome/browser/upload_list.h"

class Profile;

class WebRtcLogUploadList : public UploadList {
 public:
  
  
  static WebRtcLogUploadList* Create(Delegate* delegate, Profile* profile);

  
  static base::FilePath GetFilePathForProfile(Profile* profile);

  
  
  explicit WebRtcLogUploadList(Delegate* delegate,
                               const base::FilePath& upload_log_path);

 protected:
  virtual ~WebRtcLogUploadList();

 private:
  DISALLOW_COPY_AND_ASSIGN(WebRtcLogUploadList);
};

#endif  
