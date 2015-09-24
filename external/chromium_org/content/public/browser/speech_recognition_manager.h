// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_MANAGER_H_
#define CONTENT_PUBLIC_BROWSER_SPEECH_RECOGNITION_MANAGER_H_

#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/speech_recognition_result.h"

namespace content {

class SpeechRecognitionEventListener;
struct SpeechRecognitionSessionConfig;
struct SpeechRecognitionSessionContext;

class SpeechRecognitionManager {
 public:
  enum { kSessionIDInvalid = 0 };

  
  static CONTENT_EXPORT SpeechRecognitionManager* GetInstance();

  
  static void CONTENT_EXPORT SetManagerForTesting(
      SpeechRecognitionManager* manager);

  
  virtual int CreateSession(const SpeechRecognitionSessionConfig& config) = 0;

  
  
  virtual void StartSession(int session_id) = 0;

  
  virtual void AbortSession(int session_id) = 0;

  
  
  virtual void AbortAllSessionsForRenderProcess(int render_process_id) = 0;

  
  virtual void AbortAllSessionsForRenderView(int render_process_id,
                                             int render_view_id) = 0;

  
  
  virtual void StopAudioCaptureForSession(int session_id) = 0;

  
  
  virtual const SpeechRecognitionSessionConfig& GetSessionConfig(int session_id)
      const = 0;

  
  virtual SpeechRecognitionSessionContext GetSessionContext(
      int session_id) const = 0;

  
  
  virtual int GetSession(int render_process_id,
                         int render_view_id,
                         int request_id) const = 0;

  
  virtual bool HasAudioInputDevices() = 0;

  
  
  virtual base::string16 GetAudioInputDeviceModel() = 0;

  
  
  virtual void ShowAudioInputSettings() = 0;

 protected:
  virtual ~SpeechRecognitionManager() {}

 private:
  static SpeechRecognitionManager* manager_for_tests_;
};

}  

#endif  
