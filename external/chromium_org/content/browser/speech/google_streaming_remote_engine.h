// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_GOOGLE_STREAMING_REMOTE_ENGINE_H_
#define CONTENT_BROWSER_SPEECH_GOOGLE_STREAMING_REMOTE_ENGINE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "content/browser/speech/audio_encoder.h"
#include "content/browser/speech/chunked_byte_buffer.h"
#include "content/browser/speech/speech_recognition_engine.h"
#include "content/common/content_export.h"
#include "content/public/common/speech_recognition_error.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace net {
class URLRequestContextGetter;
}

namespace content {

class AudioChunk;
struct SpeechRecognitionError;
struct SpeechRecognitionResult;

class CONTENT_EXPORT GoogleStreamingRemoteEngine
    : public NON_EXPORTED_BASE(SpeechRecognitionEngine),
      public net::URLFetcherDelegate,
      public NON_EXPORTED_BASE(base::NonThreadSafe) {
 public:
  
  static const int kAudioPacketIntervalMs;

  
  static const int kUpstreamUrlFetcherIdForTesting;
  static const int kDownstreamUrlFetcherIdForTesting;

  explicit GoogleStreamingRemoteEngine(net::URLRequestContextGetter* context);
  virtual ~GoogleStreamingRemoteEngine();

  
  virtual void SetConfig(const SpeechRecognitionEngineConfig& config) OVERRIDE;
  virtual void StartRecognition() OVERRIDE;
  virtual void EndRecognition() OVERRIDE;
  virtual void TakeAudioChunk(const AudioChunk& data) OVERRIDE;
  virtual void AudioChunksEnded() OVERRIDE;
  virtual bool IsRecognitionPending() const OVERRIDE;
  virtual int GetDesiredAudioChunkDurationMs() const OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher* source,
                                          int64 current, int64 total) OVERRIDE;

 private:
  
  static const int kWebserviceStatusNoError;
  static const int kWebserviceStatusErrorNoMatch;

  
  enum FSMState {
    STATE_IDLE = 0,
    STATE_BOTH_STREAMS_CONNECTED,
    STATE_WAITING_DOWNSTREAM_RESULTS,
    STATE_MAX_VALUE = STATE_WAITING_DOWNSTREAM_RESULTS
  };

  enum FSMEvent {
    EVENT_END_RECOGNITION = 0,
    EVENT_START_RECOGNITION,
    EVENT_AUDIO_CHUNK,
    EVENT_AUDIO_CHUNKS_ENDED,
    EVENT_UPSTREAM_ERROR,
    EVENT_DOWNSTREAM_ERROR,
    EVENT_DOWNSTREAM_RESPONSE,
    EVENT_DOWNSTREAM_CLOSED,
    EVENT_MAX_VALUE = EVENT_DOWNSTREAM_CLOSED
  };

  struct FSMEventArgs {
    explicit FSMEventArgs(FSMEvent event_value);
    ~FSMEventArgs();

    FSMEvent event;

    
    scoped_refptr<const AudioChunk> audio_data;

    
    scoped_ptr<std::vector<uint8> > response;

   private:
    DISALLOW_COPY_AND_ASSIGN(FSMEventArgs);
  };

  
  
  void DispatchHTTPResponse(const net::URLFetcher* source,
                            bool end_of_response);

  
  void DispatchEvent(const FSMEventArgs& event_args);

  
  
  FSMState ExecuteTransitionAndGetNextState(const FSMEventArgs& event_args);

  
  FSMState ConnectBothStreams(const FSMEventArgs& event_args);
  FSMState TransmitAudioUpstream(const FSMEventArgs& event_args);
  FSMState ProcessDownstreamResponse(const FSMEventArgs& event_args);
  FSMState RaiseNoMatchErrorIfGotNoResults(const FSMEventArgs& event_args);
  FSMState CloseUpstreamAndWaitForResults(const FSMEventArgs& event_args);
  FSMState CloseDownstream(const FSMEventArgs& event_args);
  FSMState AbortSilently(const FSMEventArgs& event_args);
  FSMState AbortWithError(const FSMEventArgs& event_args);
  FSMState Abort(SpeechRecognitionErrorCode error);
  FSMState DoNothing(const FSMEventArgs& event_args);
  FSMState NotFeasible(const FSMEventArgs& event_args);

  std::string GetAcceptedLanguages() const;
  std::string GenerateRequestKey() const;

  SpeechRecognitionEngineConfig config_;
  scoped_ptr<net::URLFetcher> upstream_fetcher_;
  scoped_ptr<net::URLFetcher> downstream_fetcher_;
  scoped_refptr<net::URLRequestContextGetter> url_context_;
  scoped_ptr<AudioEncoder> encoder_;
  ChunkedByteBuffer chunked_byte_buffer_;
  size_t previous_response_length_;
  bool got_last_definitive_result_;
  bool is_dispatching_event_;
  FSMState state_;

  DISALLOW_COPY_AND_ASSIGN(GoogleStreamingRemoteEngine);
};

}  

#endif  
