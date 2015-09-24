// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_SPEECH_RECOGNITION_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_SPEECH_SPEECH_RECOGNITION_BUBBLE_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/speech/speech_recognition_bubble.h"
#include "ui/gfx/rect.h"

namespace speech {

class SpeechRecognitionBubbleController
    : public base::RefCountedThreadSafe<SpeechRecognitionBubbleController>,
      public SpeechRecognitionBubbleDelegate {
 public:
  
  class Delegate {
   public:
    
    virtual void InfoBubbleButtonClicked(
        int session_id, SpeechRecognitionBubble::Button button) = 0;

    
    
    virtual void InfoBubbleFocusChanged(int session_id) = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit SpeechRecognitionBubbleController(Delegate* delegate);

  
  void CreateBubble(int session_id,
                    int render_process_id,
                    int render_view_id,
                    const gfx::Rect& element_rect);

  
  void SetBubbleRecordingMode();

  
  void SetBubbleRecognizingMode();

  
  void SetBubbleMessage(const base::string16& text);

  
  bool IsShowingMessage() const;

  
  void SetBubbleInputVolume(float volume, float noise_volume);

  
  void CloseBubble();

  
  void CloseBubbleForRenderViewOnUIThread(int render_process_id,
                                          int render_view_id);

  
  
  int GetActiveSessionID();

  
  virtual void InfoBubbleButtonClicked(
      SpeechRecognitionBubble::Button button) OVERRIDE;
  virtual void InfoBubbleFocusChanged() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<SpeechRecognitionBubbleController>;

  
  enum RequestType {
    REQUEST_CREATE,
    REQUEST_SET_RECORDING_MODE,
    REQUEST_SET_RECOGNIZING_MODE,
    REQUEST_SET_MESSAGE,
    REQUEST_SET_INPUT_VOLUME,
    REQUEST_CLOSE,
  };

  struct UIRequest {
    RequestType type;
    base::string16 message;
    gfx::Rect element_rect;
    float volume;
    float noise_volume;
    int render_process_id;
    int render_view_id;

    explicit UIRequest(RequestType type_value);
    ~UIRequest();
  };

  virtual ~SpeechRecognitionBubbleController();

  void InvokeDelegateButtonClicked(SpeechRecognitionBubble::Button button);
  void InvokeDelegateFocusChanged();
  void ProcessRequestInUiThread(const UIRequest& request);

  
  Delegate* delegate_;
  RequestType last_request_issued_;

  
  scoped_ptr<SpeechRecognitionBubble> bubble_;

  
  base::Lock lock_;

  
  int current_bubble_session_id_;

  
  int current_bubble_render_process_id_;
  int current_bubble_render_view_id_;
};

typedef SpeechRecognitionBubbleController::Delegate
    SpeechRecognitionBubbleControllerDelegate;

}  

#endif  
