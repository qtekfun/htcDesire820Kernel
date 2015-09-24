// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_ANDROID_MEDIA_RESOURCE_GETTER_IMPL_H_
#define CONTENT_BROWSER_MEDIA_ANDROID_MEDIA_RESOURCE_GETTER_IMPL_H_

#include <jni.h>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "media/base/android/media_resource_getter.h"
#include "net/cookies/canonical_cookie.h"

namespace fileapi {
class FileSystemContext;
}

namespace net {
class URLRequestContextGetter;
}

namespace content {

class BrowserContext;
class ResourceContext;

class MediaResourceGetterImpl : public media::MediaResourceGetter {
 public:
  
  
  
  MediaResourceGetterImpl(BrowserContext* browser_context,
                          fileapi::FileSystemContext* file_system_context,
                          int renderer_id, int routing_id);
  virtual ~MediaResourceGetterImpl();

  
  
  virtual void GetCookies(const GURL& url,
                          const GURL& first_party_for_cookies,
                          const GetCookieCB& callback) OVERRIDE;
  virtual void GetPlatformPathFromFileSystemURL(
      const GURL& url,
      const GetPlatformPathCB& callback) OVERRIDE;
  virtual void ExtractMediaMetadata(
      const std::string& url, const std::string& cookies,
      const ExtractMediaMetadataCB& callback) OVERRIDE;

  static bool RegisterMediaResourceGetter(JNIEnv* env);

 private:
  
  void GetCookiesCallback(
      const GetCookieCB& callback, const std::string& cookies);

  
  void GetPlatformPathCallback(
      const GetPlatformPathCB& callback, const std::string& platform_path);

  
  BrowserContext* browser_context_;

  
  fileapi::FileSystemContext* file_system_context_;

  
  base::WeakPtrFactory<MediaResourceGetterImpl> weak_this_;

  
  int renderer_id_;

  
  int routing_id_;

  DISALLOW_COPY_AND_ASSIGN(MediaResourceGetterImpl);
};

}  

#endif  
