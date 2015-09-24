// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_USER_DATA_H_
#define CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_USER_DATA_H_

#include "base/logging.h"
#include "base/supports_user_data.h"
#include "content/public/browser/web_contents.h"

namespace content {

template <typename T>
class WebContentsUserData : public base::SupportsUserData::Data {
 public:
  
  
  static void CreateForWebContents(WebContents* contents) {
    DCHECK(contents);
    if (!FromWebContents(contents))
      contents->SetUserData(UserDataKey(), new T(contents));
  }

  
  
  
  static T* FromWebContents(WebContents* contents) {
    DCHECK(contents);
    return static_cast<T*>(contents->GetUserData(UserDataKey()));
  }
  static const T* FromWebContents(const WebContents* contents) {
    DCHECK(contents);
    return static_cast<const T*>(contents->GetUserData(UserDataKey()));
  }

 protected:
  static inline void* UserDataKey() {
    return &kLocatorKey;
  }

 private:
  
  static int kLocatorKey;
};

#define DEFINE_WEB_CONTENTS_USER_DATA_KEY(TYPE) \
template<>                                      \
int content::WebContentsUserData<TYPE>::kLocatorKey = 0

}  

#endif  
