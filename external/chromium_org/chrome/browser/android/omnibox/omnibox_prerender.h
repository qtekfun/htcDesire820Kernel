// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_OMNIBOX_OMNIBOX_PRERENDER_H_
#define CHROME_BROWSER_ANDROID_OMNIBOX_OMNIBOX_PRERENDER_H_

#include "base/android/jni_helper.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

class AutocompleteResult;
class ProfielAndroid;
class Profile;
class TabAndroid;
struct AutocompleteMatch;

namespace content {
class WebContents;
}

class OmniboxPrerender {
 public:
  OmniboxPrerender(JNIEnv* env, jobject obj);
  virtual ~OmniboxPrerender();

  
  
  
  void Clear(JNIEnv* env, jobject obj, jobject j_profile_android);

  
  
  
  
  void InitializeForProfile(JNIEnv* env,
                            jobject obj,
                            jobject j_profile_android);

  
  
  
  
  void PrerenderMaybe(JNIEnv* env,
                      jobject obj,
                      jstring j_url,
                      jstring j_current_url,
                      jint jsource_match,
                      jobject j_profile_android,
                      jobject j_tab);

 private:

  
  void DoPrerender(const AutocompleteMatch& match,
                   Profile* profile,
                   content::WebContents* web_contents);
  JavaObjectWeakGlobalRef weak_java_omnibox_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxPrerender);
};

bool RegisterOmniboxPrerender(JNIEnv* env);

#endif  
