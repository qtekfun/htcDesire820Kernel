// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_TEXT_RENDERER_H_
#define MEDIA_BASE_TEXT_RENDERER_H_

#include <map>
#include <set>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"
#include "media/base/text_track.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class TextCue;
class TextTrackConfig;

class MEDIA_EXPORT TextRenderer {
 public:
  
  
  
  
  TextRenderer(const scoped_refptr<base::MessageLoopProxy>& message_loop,
               const AddTextTrackCB& add_text_track_cb);
  ~TextRenderer();

  
  
  void Initialize(const base::Closure& ended_cb);

  
  
  void Play(const base::Closure& callback);

  
  
  void Pause(const base::Closure& callback);

  
  void Flush(const base::Closure& callback);

  
  
  void Stop(const base::Closure& callback);

  
  
  void AddTextStream(DemuxerStream* text_stream,
                     const TextTrackConfig& config);

  
  void RemoveTextStream(DemuxerStream* text_stream);

  
  bool HasTracks() const;

 private:
  struct TextTrackState {
    
    enum ReadState {
      kReadIdle,
      kReadPending
    };

    explicit TextTrackState(scoped_ptr<TextTrack> text_track);
    ~TextTrackState();

    ReadState read_state;
    scoped_ptr<TextTrack> text_track;
  };

  
  
  void BufferReady(DemuxerStream* text_stream,
                   DemuxerStream::Status status,
                   const scoped_refptr<DecoderBuffer>& input);

  
  void CueReady(DemuxerStream* text_stream,
                const scoped_refptr<TextCue>& text_cue);

  
  
  void OnAddTextTrackDone(DemuxerStream* text_stream,
                          scoped_ptr<TextTrack> text_track);

  
  void Read(TextTrackState* state, DemuxerStream* text_stream);

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<TextRenderer> weak_factory_;
  base::WeakPtr<TextRenderer> weak_this_;
  const AddTextTrackCB add_text_track_cb_;

  
  base::Closure ended_cb_;

  
  base::Closure pause_cb_;

  
  base::Closure stop_cb_;

  
  enum State {
    kUninitialized,
    kPausePending,
    kPaused,
    kPlaying,
    kEnded,
    kStopPending,
    kStopped
  };
  State state_;

  typedef std::map<DemuxerStream*, TextTrackState*> TextTrackStateMap;
  TextTrackStateMap text_track_state_map_;

  
  int pending_read_count_;

  
  typedef std::set<DemuxerStream*> PendingEosSet;
  PendingEosSet pending_eos_set_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(TextRenderer);
};

}  

#endif  
