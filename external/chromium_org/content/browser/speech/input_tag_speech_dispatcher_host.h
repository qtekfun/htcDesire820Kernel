// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_INPUT_TAG_SPEECH_DISPATCHER_HOST_H_
#define CONTENT_BROWSER_SPEECH_INPUT_TAG_SPEECH_DISPATCHER_HOST_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"
#include "content/public/browser/speech_recognition_event_listener.h"
#include "content/public/common/speech_recognition_result.h"
#include "net/url_request/url_request_context_getter.h"

struct InputTagSpeechHostMsg_StartRecognition_Params;

namespace content {

class SpeechRecognitionManager;

class CONTENT_EXPORT InputTagSpeechDispatcherHost
    : public BrowserMessageFilter,
      public SpeechRecognitionEventListener {
 public:
  InputTagSpeechDispatcherHost(
      bool guest,
      int render_process_id,
      net::URLRequestContextGetter* url_request_context_getter);

  base::WeakPtr<InputTagSpeechDispatcherHost> AsWeakPtr();

  
  virtual void OnRecognitionStart(int session_id) OVERRIDE;
  virtual void OnAudioStart(int session_id) OVERRIDE;
  virtual void OnEnvironmentEstimationComplete(int session_id) OVERRIDE;
  virtual void OnSoundStart(int session_id) OVERRIDE;
  virtual void OnSoundEnd(int session_id) OVERRIDE;
  virtual void OnAudioEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionResults(
      int session_id,
      const SpeechRecognitionResults& results) OVERRIDE;
  virtual void OnRecognitionError(
      int session_id,
      const SpeechRecognitionError& error) OVERRIDE;
  virtual void OnAudioLevelsChange(int session_id,
                                   float volume,
                                   float noise_volume) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;
  virtual void OverrideThreadForMessage(
      const IPC::Message& message,
      BrowserThread::ID* thread) OVERRIDE;

  virtual void OnChannelClosing() OVERRIDE;

 private:
  virtual ~InputTagSpeechDispatcherHost();

  void OnStartRecognition(
      const InputTagSpeechHostMsg_StartRecognition_Params& params);
  void OnCancelRecognition(int render_view_id, int request_id);
  void OnStopRecording(int render_view_id, int request_id);

  void StartRecognitionOnIO(
      int embedder_render_process_id,
      int embedder_render_view_id,
      const InputTagSpeechHostMsg_StartRecognition_Params& params,
      bool filter_profanities);

  bool is_guest_;
  int render_process_id_;
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;

  
  
  
  base::WeakPtrFactory<InputTagSpeechDispatcherHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(InputTagSpeechDispatcherHost);
};

}  

#endif  
