// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_SPEECH_INPUT_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_SPEECH_SPEECH_INPUT_BUBBLE_CONTROLLER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/speech/speech_input_bubble.h"
#include "content/common/notification_observer.h"

namespace gfx {
class Rect;
}
class NotificationRegistrar;

namespace speech_input {

class SpeechInputBubbleController
    : public base::RefCountedThreadSafe<SpeechInputBubbleController>,
      public SpeechInputBubbleDelegate,
      public NotificationObserver {
 public:
  
  class Delegate {
   public:
    
    virtual void InfoBubbleButtonClicked(int caller_id,
                                         SpeechInputBubble::Button button) = 0;

    
    
    virtual void InfoBubbleFocusChanged(int caller_id) = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit SpeechInputBubbleController(Delegate* delegate);
  virtual ~SpeechInputBubbleController();

  
  
  void CreateBubble(int caller_id,
                    int render_process_id,
                    int render_view_id,
                    const gfx::Rect& element_rect);

  
  
  void SetBubbleWarmUpMode(int caller_id);

  
  
  void SetBubbleRecordingMode(int caller_id);

  
  
  void SetBubbleRecognizingMode(int caller_id);

  
  
  void SetBubbleMessage(int caller_id, const string16& text);

  
  void SetBubbleInputVolume(int caller_id, float volume, float noise_volume);

  void CloseBubble(int caller_id);

  
  virtual void InfoBubbleButtonClicked(SpeechInputBubble::Button button);
  virtual void InfoBubbleFocusChanged();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  enum RequestType {
    REQUEST_SET_WARM_UP_MODE,
    REQUEST_SET_RECORDING_MODE,
    REQUEST_SET_RECOGNIZING_MODE,
    REQUEST_SET_MESSAGE,
    REQUEST_SET_INPUT_VOLUME,
    REQUEST_CLOSE,
  };

  enum ManageSubscriptionAction {
    BUBBLE_ADDED,
    BUBBLE_REMOVED
  };

  void InvokeDelegateButtonClicked(int caller_id,
                                   SpeechInputBubble::Button button);
  void InvokeDelegateFocusChanged(int caller_id);
  void ProcessRequestInUiThread(int caller_id,
                                RequestType type,
                                const string16& text,
                                float volume,
                                float noise_volume);

  
  
  
  
  
  void UpdateTabContentsSubscription(int caller_id,
                                     ManageSubscriptionAction action);

  
  Delegate* delegate_;

  

  
  
  int current_bubble_caller_id_;

  
  
  typedef std::map<int, SpeechInputBubble*> BubbleCallerIdMap;
  BubbleCallerIdMap bubbles_;

  scoped_ptr<NotificationRegistrar> registrar_;
};

typedef SpeechInputBubbleController::Delegate
    SpeechInputBubbleControllerDelegate;

}  

#endif  
