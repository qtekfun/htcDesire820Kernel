// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_PROVIDER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_PROVIDER_H_

#include <list>
#include <string>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

enum MediaStreamProviderError {
  kMediaStreamOk = 0,
  kInvalidMediaStreamType,
  kInvalidSession,
  kUnknownSession,
  kDeviceNotAvailable,
  kDeviceAlreadyInUse,
  kUnknownError
};

enum { kInvalidMediaCaptureSessionId = 0xFFFFFFFF };

class CONTENT_EXPORT MediaStreamProviderListener {
 public:
  
  virtual void Opened(MediaStreamType stream_type,
                      int capture_session_id) = 0;

  
  virtual void Closed(MediaStreamType stream_type,
                      int capture_session_id) = 0;

  
  virtual void DevicesEnumerated(MediaStreamType stream_type,
                                 const StreamDeviceInfoArray& devices) = 0;

 protected:
  virtual ~MediaStreamProviderListener() {}
};

class CONTENT_EXPORT MediaStreamProvider
    : public base::RefCountedThreadSafe<MediaStreamProvider> {
 public:
  
  virtual void Register(MediaStreamProviderListener* listener,
                        base::MessageLoopProxy* device_thread_loop) = 0;

  
  virtual void Unregister() = 0;

  
  virtual void EnumerateDevices(MediaStreamType stream_type) = 0;

  
  
  
  
  virtual int Open(const StreamDeviceInfo& device) = 0;

  
  virtual void Close(int capture_session_id) = 0;

 protected:
  friend class base::RefCountedThreadSafe<MediaStreamProvider>;
  virtual ~MediaStreamProvider() {}
};

}  

#endif  
