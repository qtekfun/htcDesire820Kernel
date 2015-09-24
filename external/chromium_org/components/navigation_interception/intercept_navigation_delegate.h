// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NAVIGATION_INTERCEPTION_INTERCEPT_NAVIGATION_DELEGATE_H_
#define COMPONENTS_NAVIGATION_INTERCEPTION_INTERCEPT_NAVIGATION_DELEGATE_H_

#include "base/android/jni_helper.h"
#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "content/public/common/page_transition_types.h"

class GURL;

namespace content {
class ResourceThrottle;
class WebContents;
}

namespace net {
class URLRequest;
}

namespace navigation_interception {

class NavigationParams;

class InterceptNavigationDelegate : public base::SupportsUserData::Data {
 public:
  InterceptNavigationDelegate(JNIEnv* env, jobject jdelegate);
  virtual ~InterceptNavigationDelegate();

  
  
  
  
  static void Associate(content::WebContents* web_contents,
                        scoped_ptr<InterceptNavigationDelegate> delegate);
  
  
  static InterceptNavigationDelegate* Get(content::WebContents* web_contents);

  
  
  static content::ResourceThrottle* CreateThrottleFor(
      net::URLRequest* request);

  virtual bool ShouldIgnoreNavigation(
      const NavigationParams& navigation_params);
 private:
  JavaObjectWeakGlobalRef weak_jdelegate_;
};

bool RegisterInterceptNavigationDelegate(JNIEnv* env);

}  

#endif  
