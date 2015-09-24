// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_TTS_CONTROLLER_H_
#define CHROME_BROWSER_SPEECH_TTS_CONTROLLER_H_

#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "url/gurl.h"

class Utterance;
class TtsPlatformImpl;
class Profile;

namespace base {
class Value;
}

enum TtsEventType {
  TTS_EVENT_START,
  TTS_EVENT_END,
  TTS_EVENT_WORD,
  TTS_EVENT_SENTENCE,
  TTS_EVENT_MARKER,
  TTS_EVENT_INTERRUPTED,
  TTS_EVENT_CANCELLED,
  TTS_EVENT_ERROR,
  TTS_EVENT_PAUSE,
  TTS_EVENT_RESUME
};

enum TtsGenderType {
  TTS_GENDER_NONE,
  TTS_GENDER_MALE,
  TTS_GENDER_FEMALE
};

bool IsFinalTtsEventType(TtsEventType event_type);

struct UtteranceContinuousParameters {
  UtteranceContinuousParameters();

  double rate;
  double pitch;
  double volume;
};

struct VoiceData {
  VoiceData();
  ~VoiceData();

  std::string name;
  std::string lang;
  TtsGenderType gender;
  std::string extension_id;
  std::set<TtsEventType> events;

  
  
  bool remote;

  
  
  bool native;
  std::string native_voice_identifier;
};

class UtteranceEventDelegate {
 public:
  virtual ~UtteranceEventDelegate() {}
  virtual void OnTtsEvent(Utterance* utterance,
                          TtsEventType event_type,
                          int char_index,
                          const std::string& error_message) = 0;
};

class VoicesChangedDelegate {
 public:
  virtual ~VoicesChangedDelegate() {}
  virtual void OnVoicesChanged() = 0;
};

class Utterance {
 public:
  
  
  
  explicit Utterance(Profile* profile);
  ~Utterance();

  
  
  
  void OnTtsEvent(TtsEventType event_type,
                  int char_index,
                  const std::string& error_message);

  
  void Finish();

  
  void set_text(const std::string& text) { text_ = text; }
  const std::string& text() const { return text_; }

  void set_options(const base::Value* options);
  const base::Value* options() const { return options_.get(); }

  void set_src_extension_id(const std::string& src_extension_id) {
    src_extension_id_ = src_extension_id;
  }
  const std::string& src_extension_id() { return src_extension_id_; }

  void set_src_id(int src_id) { src_id_ = src_id; }
  int src_id() { return src_id_; }

  void set_src_url(const GURL& src_url) { src_url_ = src_url; }
  const GURL& src_url() { return src_url_; }

  void set_voice_name(const std::string& voice_name) {
    voice_name_ = voice_name;
  }
  const std::string& voice_name() const { return voice_name_; }

  void set_lang(const std::string& lang) {
    lang_ = lang;
  }
  const std::string& lang() const { return lang_; }

  void set_gender(TtsGenderType gender) {
    gender_ = gender;
  }
  TtsGenderType gender() const { return gender_; }

  void set_continuous_parameters(const UtteranceContinuousParameters& params) {
    continuous_parameters_ = params;
  }
  const UtteranceContinuousParameters& continuous_parameters() {
    return continuous_parameters_;
  }

  void set_can_enqueue(bool can_enqueue) { can_enqueue_ = can_enqueue; }
  bool can_enqueue() const { return can_enqueue_; }

  void set_required_event_types(const std::set<TtsEventType>& types) {
    required_event_types_ = types;
  }
  const std::set<TtsEventType>& required_event_types() const {
    return required_event_types_;
  }

  void set_desired_event_types(const std::set<TtsEventType>& types) {
    desired_event_types_ = types;
  }
  const std::set<TtsEventType>& desired_event_types() const {
    return desired_event_types_;
  }

  const std::string& extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& extension_id) {
    extension_id_ = extension_id;
  }

  UtteranceEventDelegate* event_delegate() const {
    return event_delegate_.get();
  }
  void set_event_delegate(
      base::WeakPtr<UtteranceEventDelegate> event_delegate) {
    event_delegate_ = event_delegate;
  }

  
  Profile* profile() const { return profile_; }
  int id() const { return id_; }
  bool finished() const { return finished_; }

 private:
  
  Profile* profile_;

  
  
  std::string extension_id_;

  
  
  int id_;

  
  
  static int next_utterance_id_;

  
  std::string text_;

  
  
  scoped_ptr<base::Value> options_;

  
  
  std::string src_extension_id_;

  
  
  int src_id_;

  
  GURL src_url_;

  
  base::WeakPtr<UtteranceEventDelegate> event_delegate_;

  
  std::string voice_name_;
  std::string lang_;
  TtsGenderType gender_;
  UtteranceContinuousParameters continuous_parameters_;
  bool can_enqueue_;
  std::set<TtsEventType> required_event_types_;
  std::set<TtsEventType> desired_event_types_;

  
  int char_index_;

  
  bool finished_;
};

class TtsController {
 public:
  
  static TtsController* GetInstance();

  
  bool IsSpeaking();

  
  
  
  
  void SpeakOrEnqueue(Utterance* utterance);

  
  
  void Stop();

  
  
  void Pause();

  
  void Resume();

  
  
  
  
  void OnTtsEvent(int utterance_id,
                  TtsEventType event_type,
                  int char_index,
                  const std::string& error_message);

  
  
  void GetVoices(Profile* profile, std::vector<VoiceData>* out_voices);

  
  
  void RetrySpeakingQueuedUtterances();

  
  
  void VoicesChanged();

  
  void AddVoicesChangedDelegate(VoicesChangedDelegate* delegate);

  
  void RemoveVoicesChangedDelegate(VoicesChangedDelegate* delegate);

  
  void SetPlatformImpl(TtsPlatformImpl* platform_impl);
  int QueueSize();

 protected:
  TtsController();
  virtual ~TtsController();

 private:
  
  TtsPlatformImpl* GetPlatformImpl();

  
  
  void SpeakNow(Utterance* utterance);

  
  
  void ClearUtteranceQueue(bool send_events);

  
  void FinishCurrentUtterance();

  
  void SpeakNextUtterance();

  
  
  int GetMatchingVoice(const Utterance* utterance,
                       std::vector<VoiceData>& voices);

  friend struct DefaultSingletonTraits<TtsController>;

  
  Utterance* current_utterance_;

  
  bool paused_;

  
  std::queue<Utterance*> utterance_queue_;

  
  std::set<VoicesChangedDelegate*> voices_changed_delegates_;

  
  
  TtsPlatformImpl* platform_impl_;

  DISALLOW_COPY_AND_ASSIGN(TtsController);
};

#endif  
