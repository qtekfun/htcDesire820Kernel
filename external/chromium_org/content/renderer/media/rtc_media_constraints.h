// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RTC_MEDIA_CONSTRAINTS_H_
#define CONTENT_RENDERER_MEDIA_RTC_MEDIA_CONSTRAINTS_H_

#include "base/compiler_specific.h"
#include "content/common/content_export.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediaconstraintsinterface.h"

namespace blink {
class WebMediaConstraints;
}

namespace content {

class CONTENT_EXPORT RTCMediaConstraints
    : public NON_EXPORTED_BASE(webrtc::MediaConstraintsInterface) {
 public:
  RTCMediaConstraints();
  explicit RTCMediaConstraints(
      const blink::WebMediaConstraints& constraints);
  virtual ~RTCMediaConstraints();
  virtual const Constraints& GetMandatory() const OVERRIDE;
  virtual const Constraints& GetOptional() const OVERRIDE;
  void AddOptional(const std::string& key, const std::string& value);
  
  
  
  bool AddMandatory(const std::string& key, const std::string& value,
                    bool override_if_exists);

 protected:
  Constraints mandatory_;
  Constraints optional_;
};

}  


#endif  
