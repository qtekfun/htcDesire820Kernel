// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TTS_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TTS_API_H_

#include <queue>
#include <string>

#include "base/memory/singleton.h"
#include "base/task.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/extensions/extension_tts_api_util.h"

class ExtensionTtsPlatformImpl {
 public:
  static ExtensionTtsPlatformImpl* GetInstance();

  
  
  
  
  
  
  
  
  
  virtual bool Speak(
      const std::string& utterance,
      const std::string& locale,
      const std::string& gender,
      double rate,
      double pitch,
      double volume) = 0;

  
  virtual bool StopSpeaking() = 0;

  
  virtual bool IsSpeaking() = 0;

  virtual std::string error();
  virtual void clear_error();
  virtual void set_error(const std::string& error);

 protected:
  ExtensionTtsPlatformImpl() {}
  virtual ~ExtensionTtsPlatformImpl() {}

  std::string error_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionTtsPlatformImpl);
};

class Utterance {
 public:
  
  
  
  Utterance(Profile* profile,
            const std::string& text,
            DictionaryValue* options,
            Task* completion_task);
  ~Utterance();

  
  void FinishAndDestroy();

  void set_error(const std::string& error) { error_ = error; }
  void set_extension_id(const std::string& extension_id) {
    extension_id_ = extension_id;
  }

  
  Profile* profile() { return profile_; }
  const std::string& extension_id() { return extension_id_; }
  int id() { return id_; }
  const std::string& text() { return text_; }
  const Value* options() { return options_.get(); }
  const std::string& voice_name() { return voice_name_; }
  const std::string& locale() { return locale_; }
  const std::string& gender() { return gender_; }
  double rate() { return rate_; }
  double pitch() { return pitch_; }
  double volume() { return volume_; }
  bool can_enqueue() { return can_enqueue_; }
  const std::string& error() { return error_; }

 private:
  
  Profile* profile_;

  
  
  std::string extension_id_;

  
  
  int id_;

  
  
  static int next_utterance_id_;

  
  std::string text_;

  
  
  scoped_ptr<Value> options_;

  
  std::string voice_name_;
  std::string locale_;
  std::string gender_;
  double rate_;
  double pitch_;
  double volume_;
  bool can_enqueue_;

  
  
  std::string error_;

  
  Task* completion_task_;
};

class ExtensionTtsController {
 public:
  
  static ExtensionTtsController* GetInstance();

  
  bool IsSpeaking() const;

  
  
  
  
  void SpeakOrEnqueue(Utterance* utterance);

  
  void Stop();

  
  void OnSpeechFinished(int request_id, const std::string& error_message);

  
  void SetPlatformImpl(ExtensionTtsPlatformImpl* platform_impl);

 private:
  ExtensionTtsController();
  virtual ~ExtensionTtsController();

  
  ExtensionTtsPlatformImpl* GetPlatformImpl();

  
  
  void SpeakNow(Utterance* utterance);

  
  
  
  void CheckSpeechStatus();

  
  void ClearUtteranceQueue();

  
  void FinishCurrentUtterance();

  
  void SpeakNextUtterance();

  
  
  
  std::string GetMatchingExtensionId(Utterance* utterance);

  ScopedRunnableMethodFactory<ExtensionTtsController> method_factory_;
  friend struct DefaultSingletonTraits<ExtensionTtsController>;

  
  Utterance* current_utterance_;

  
  std::queue<Utterance*> utterance_queue_;

  
  
  ExtensionTtsPlatformImpl* platform_impl_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionTtsController);
};


class ExtensionTtsSpeakFunction : public AsyncExtensionFunction {
 private:
  ~ExtensionTtsSpeakFunction() {}
  virtual bool RunImpl();
  void SpeechFinished();
  Utterance* utterance_;
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.tts.speak")
};

class ExtensionTtsStopSpeakingFunction : public SyncExtensionFunction {
 private:
  ~ExtensionTtsStopSpeakingFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.tts.stop")
};

class ExtensionTtsIsSpeakingFunction : public SyncExtensionFunction {
 private:
  ~ExtensionTtsIsSpeakingFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.tts.isSpeaking")
};

class ExtensionTtsSpeakCompletedFunction : public SyncExtensionFunction {
 private:
  ~ExtensionTtsSpeakCompletedFunction() {}
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.tts.speakCompleted")
};

#endif  
