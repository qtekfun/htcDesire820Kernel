// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_DATE_TIME_CHOOSER_ANDROID_H_
#define CONTENT_BROWSER_ANDROID_DATE_TIME_CHOOSER_ANDROID_H_

#include <string>
#include <vector>

#include "base/android/jni_helper.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/ime/text_input_type.h"

namespace content {

class ContentViewCore;
class RenderViewHost;
struct DateTimeSuggestion;

class DateTimeChooserAndroid {
 public:
  DateTimeChooserAndroid();
  ~DateTimeChooserAndroid();

  
  
  
  void ShowDialog(ContentViewCore* content,
                  RenderViewHost* host,
                  ui::TextInputType dialog_type,
                  double dialog_value,
                  double min,
                  double max,
                  double step,
                  const std::vector<DateTimeSuggestion>& suggestions);

  
  void ReplaceDateTime(JNIEnv* env, jobject, jdouble value);

  
  void CancelDialog(JNIEnv* env, jobject);

  
  
  static void InitializeDateInputTypes(
       int text_input_type_date, int text_input_type_date_time,
       int text_input_type_date_time_local, int text_input_type_month,
       int text_input_type_time, int text_input_type_week);

 private:
  RenderViewHost* host_;

  base::android::ScopedJavaGlobalRef<jobject> j_date_time_chooser_;

  DISALLOW_COPY_AND_ASSIGN(DateTimeChooserAndroid);
};

bool RegisterDateTimeChooserAndroid(JNIEnv* env);

}  

#endif  
