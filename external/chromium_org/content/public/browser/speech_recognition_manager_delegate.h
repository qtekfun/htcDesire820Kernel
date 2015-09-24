// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_MANAGER_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_MANAGER_DELEGATE_H_

#include <string>

#include "base/callback_forward.h"
#include "content/public/common/speech_recognition_error.h"

namespace content {

class SpeechRecognitionEventListener;
struct SpeechRecognitionResult;

class SpeechRecognitionManagerDelegate {
 public:
  virtual ~SpeechRecognitionManagerDelegate() {}

  
  
  virtual void GetDiagnosticInformation(bool* can_report_metrics,
                                        std::string* hardware_info) = 0;

  
  
  virtual void CheckRecognitionIsAllowed(
      int session_id,
      base::Callback<void(bool ask_user, bool is_allowed)> callback) = 0;

  
  
  
  virtual SpeechRecognitionEventListener* GetEventListener() = 0;

  
  
  virtual bool FilterProfanities(int render_process_id) = 0;
};

}  

#endif  
