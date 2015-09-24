// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_SPEECH_RECOGNITION_MANAGER_IMPL_H_
#define CONTENT_BROWSER_SPEECH_SPEECH_RECOGNITION_MANAGER_IMPL_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/renderer_host/media/media_stream_requester.h"
#include "content/public/browser/speech_recognition_event_listener.h"
#include "content/public/browser/speech_recognition_manager.h"
#include "content/public/browser/speech_recognition_session_config.h"
#include "content/public/browser/speech_recognition_session_context.h"
#include "content/public/common/speech_recognition_error.h"

namespace media {
class AudioManager;
}

namespace content {
class BrowserMainLoop;
class MediaStreamManager;
class MediaStreamUIProxy;
class SpeechRecognitionManagerDelegate;
class SpeechRecognizer;

class CONTENT_EXPORT SpeechRecognitionManagerImpl :
    public NON_EXPORTED_BASE(SpeechRecognitionManager),
    public SpeechRecognitionEventListener {
 public:
  
  
  static SpeechRecognitionManagerImpl* GetInstance();

  
  virtual int CreateSession(
      const SpeechRecognitionSessionConfig& config) OVERRIDE;
  virtual void StartSession(int session_id) OVERRIDE;
  virtual void AbortSession(int session_id) OVERRIDE;
  virtual void AbortAllSessionsForRenderProcess(int render_process_id) OVERRIDE;
  virtual void AbortAllSessionsForRenderView(int render_process_id,
                                             int render_view_id) OVERRIDE;
  virtual void StopAudioCaptureForSession(int session_id) OVERRIDE;
  virtual const SpeechRecognitionSessionConfig& GetSessionConfig(
      int session_id) const OVERRIDE;
  virtual SpeechRecognitionSessionContext GetSessionContext(
      int session_id) const OVERRIDE;
  virtual int GetSession(int render_process_id,
                         int render_view_id,
                         int request_id) const OVERRIDE;
  virtual bool HasAudioInputDevices() OVERRIDE;
  virtual base::string16 GetAudioInputDeviceModel() OVERRIDE;
  virtual void ShowAudioInputSettings() OVERRIDE;

  
  virtual void OnRecognitionStart(int session_id) OVERRIDE;
  virtual void OnAudioStart(int session_id) OVERRIDE;
  virtual void OnEnvironmentEstimationComplete(int session_id) OVERRIDE;
  virtual void OnSoundStart(int session_id) OVERRIDE;
  virtual void OnSoundEnd(int session_id) OVERRIDE;
  virtual void OnAudioEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionEnd(int session_id) OVERRIDE;
  virtual void OnRecognitionResults(
      int session_id, const SpeechRecognitionResults& result) OVERRIDE;
  virtual void OnRecognitionError(
      int session_id, const SpeechRecognitionError& error) OVERRIDE;
  virtual void OnAudioLevelsChange(int session_id, float volume,
                                   float noise_volume) OVERRIDE;

  SpeechRecognitionManagerDelegate* delegate() const { return delegate_.get(); }

 protected:
  
  friend class BrowserMainLoop;
  
  friend struct base::DefaultDeleter<SpeechRecognitionManagerImpl>;
  SpeechRecognitionManagerImpl(media::AudioManager* audio_manager,
                               MediaStreamManager* media_stream_manager);
  virtual ~SpeechRecognitionManagerImpl();

 private:
  
  enum FSMState {
    SESSION_STATE_IDLE = 0,
    SESSION_STATE_CAPTURING_AUDIO,
    SESSION_STATE_WAITING_FOR_RESULT,
    SESSION_STATE_MAX_VALUE = SESSION_STATE_WAITING_FOR_RESULT
  };

  enum FSMEvent {
    EVENT_ABORT = 0,
    EVENT_START,
    EVENT_STOP_CAPTURE,
    EVENT_AUDIO_ENDED,
    EVENT_RECOGNITION_ENDED,
    EVENT_MAX_VALUE = EVENT_RECOGNITION_ENDED
  };

  struct Session {
    Session();
    ~Session();

    int id;
    bool abort_requested;
    bool listener_is_active;
    SpeechRecognitionSessionConfig config;
    SpeechRecognitionSessionContext context;
    scoped_refptr<SpeechRecognizer> recognizer;
    scoped_ptr<MediaStreamUIProxy> ui;
  };

  
  
  void RecognitionAllowedCallback(int session_id,
                                  bool ask_user,
                                  bool is_allowed);

  
  
  
  void MediaRequestPermissionCallback(int session_id,
                                      const MediaStreamDevices& devices,
                                      scoped_ptr<MediaStreamUIProxy> stream_ui);

  
  void DispatchEvent(int session_id, FSMEvent event);

  
  
  void ExecuteTransitionAndGetNextState(Session* session,
                                        FSMState session_state,
                                        FSMEvent event);

  
  FSMState GetSessionState(int session_id) const;

  
  void SessionStart(const Session& session);
  void SessionAbort(const Session& session);
  void SessionStopAudioCapture(const Session& session);
  void ResetCapturingSessionId(const Session& session);
  void SessionDelete(Session* session);
  void NotFeasible(const Session& session, FSMEvent event);

  bool SessionExists(int session_id) const;
  Session* GetSession(int session_id) const;
  SpeechRecognitionEventListener* GetListener(int session_id) const;
  SpeechRecognitionEventListener* GetDelegateListener() const;
  int GetNextSessionID();

  media::AudioManager* audio_manager_;
  MediaStreamManager* media_stream_manager_;
  typedef std::map<int, Session*> SessionsTable;
  SessionsTable sessions_;
  int primary_session_id_;
  int last_session_id_;
  bool is_dispatching_event_;
  scoped_ptr<SpeechRecognitionManagerDelegate> delegate_;

  
  
  
  base::WeakPtrFactory<SpeechRecognitionManagerImpl> weak_factory_;
};

}  

#endif  
