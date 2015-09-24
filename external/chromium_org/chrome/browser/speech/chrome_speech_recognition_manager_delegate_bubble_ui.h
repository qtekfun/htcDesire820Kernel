// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_CHROME_SPEECH_RECOGNITION_MANAGER_DELEGATE_BUBBLE_UI_H_
#define CHROME_BROWSER_SPEECH_CHROME_SPEECH_RECOGNITION_MANAGER_DELEGATE_BUBBLE_UI_H_

#include "base/compiler_specific.h"
#include "chrome/browser/speech/chrome_speech_recognition_manager_delegate.h"
#include "chrome/browser/speech/speech_recognition_bubble_controller.h"

namespace speech {

class ChromeSpeechRecognitionManagerDelegateBubbleUI
    : public ChromeSpeechRecognitionManagerDelegate,
      public SpeechRecognitionBubbleControllerDelegate {
 public:
  ChromeSpeechRecognitionManagerDelegateBubbleUI();
  virtual ~ChromeSpeechRecognitionManagerDelegateBubbleUI();

 protected:
  
  virtual void InfoBubbleButtonClicked(
      int session_id, SpeechRecognitionBubble::Button button) OVERRIDE;
  virtual void InfoBubbleFocusChanged(int session_id) OVERRIDE;

  
  virtual void OnRecognitionStart(int session_id) OVERRIDE;
  virtual void OnAudioStart(int session_id) OVERRIDE;
  virtual void OnAudioEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionError(
      int session_id, const content::SpeechRecognitionError& error) OVERRIDE;
  virtual void OnAudioLevelsChange(
      int session_id, float volume, float noise_volume) OVERRIDE;

  
  
  void RestartLastSession();

  
  virtual void TabClosedCallback(
      int render_process_id, int render_view_id) OVERRIDE;

 private:
  
  SpeechRecognitionBubbleController* GetBubbleController();

  scoped_refptr<SpeechRecognitionBubbleController> bubble_controller_;
  scoped_ptr<content::SpeechRecognitionSessionConfig> last_session_config_;

  DISALLOW_COPY_AND_ASSIGN(ChromeSpeechRecognitionManagerDelegateBubbleUI);
};

}  

#endif  
