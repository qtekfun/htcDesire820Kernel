// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef WEBKIT_GLUE_WEBMEDIAPLAYER_IMPL_H_
#define WEBKIT_GLUE_WEBMEDIAPLAYER_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/threading/thread.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "media/base/filters.h"
#include "media/base/message_loop_factory.h"
#include "media/base/pipeline.h"
#include "skia/ext/platform_canvas.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebMediaPlayer.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebMediaPlayerClient.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "webkit/glue/media/web_data_source.h"

class GURL;

namespace WebKit {
class WebFrame;
}

namespace webkit_glue {

class MediaResourceLoaderBridgeFactory;
class WebVideoRenderer;

class WebMediaPlayerImpl : public WebKit::WebMediaPlayer,
                           public MessageLoop::DestructionObserver {
 public:
  
  
  
  
  
  
  
  
  class Proxy : public base::RefCountedThreadSafe<Proxy> {
   public:
    Proxy(MessageLoop* render_loop,
          WebMediaPlayerImpl* webmediaplayer);

    
    void Repaint();
    void SetVideoRenderer(scoped_refptr<WebVideoRenderer> video_renderer);
    WebDataSourceBuildObserverHack* GetBuildObserver();

    
    void Paint(SkCanvas* canvas, const gfx::Rect& dest_rect);
    void SetSize(const gfx::Rect& rect);
    void Detach();
    void GetCurrentFrame(scoped_refptr<media::VideoFrame>* frame_out);
    void PutCurrentFrame(scoped_refptr<media::VideoFrame> frame);
    bool HasSingleOrigin();
    void AbortDataSources();

    
    void PipelineInitializationCallback(media::PipelineStatus status);
    void PipelineSeekCallback(media::PipelineStatus status);
    void PipelineEndedCallback(media::PipelineStatus status);
    void PipelineErrorCallback(media::PipelineStatus error);
    void NetworkEventCallback(media::PipelineStatus status);

    
    MessageLoop* message_loop() { return render_loop_; }

   private:
    friend class base::RefCountedThreadSafe<Proxy>;

    virtual ~Proxy();

    
    void AddDataSource(WebDataSource* data_source);

    
    void RepaintTask();

    
    void PipelineInitializationTask(media::PipelineStatus status);

    
    void PipelineSeekTask(media::PipelineStatus status);

    
    void PipelineEndedTask(media::PipelineStatus status);

    
    
    void PipelineErrorTask(media::PipelineStatus error);

    
    void NetworkEventTask(media::PipelineStatus status);

    
    MessageLoop* render_loop_;
    WebMediaPlayerImpl* webmediaplayer_;

    base::Lock data_sources_lock_;
    typedef std::list<scoped_refptr<WebDataSource> > DataSourceList;
    DataSourceList data_sources_;
    scoped_ptr<WebDataSourceBuildObserverHack> build_observer_;

    scoped_refptr<WebVideoRenderer> video_renderer_;

    base::Lock lock_;
    int outstanding_repaints_;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  WebMediaPlayerImpl(WebKit::WebMediaPlayerClient* client,
                     media::FilterCollection* collection,
                     media::MessageLoopFactory* message_loop_factory);
  virtual ~WebMediaPlayerImpl();

  
  bool Initialize(
      WebKit::WebFrame* frame,
      bool use_simple_data_source,
      scoped_refptr<WebVideoRenderer> web_video_renderer);

  virtual void load(const WebKit::WebURL& url);
  virtual void cancelLoad();

  
  virtual void play();
  virtual void pause();
  virtual bool supportsFullscreen() const;
  virtual bool supportsSave() const;
  virtual void seek(float seconds);
  virtual void setEndTime(float seconds);
  virtual void setRate(float rate);
  virtual void setVolume(float volume);
  virtual void setVisible(bool visible);
  virtual void setPreload(WebKit::WebMediaPlayer::Preload preload);
  virtual bool totalBytesKnown();
  virtual const WebKit::WebTimeRanges& buffered();
  virtual float maxTimeSeekable() const;

  
  virtual void setSize(const WebKit::WebSize& size);

  virtual void paint(WebKit::WebCanvas* canvas, const WebKit::WebRect& rect);

  
  virtual bool hasVideo() const;
  virtual bool hasAudio() const;

  
  virtual WebKit::WebSize naturalSize() const;

  
  virtual bool paused() const;
  virtual bool seeking() const;
  virtual float duration() const;
  virtual float currentTime() const;

  
  virtual int32 dataRate() const;

  
  
  
  virtual WebKit::WebMediaPlayer::NetworkState networkState() const;
  virtual WebKit::WebMediaPlayer::ReadyState readyState() const;

  virtual unsigned long long bytesLoaded() const;
  virtual unsigned long long totalBytes() const;

  virtual bool hasSingleSecurityOrigin() const;
  virtual WebKit::WebMediaPlayer::MovieLoadType movieLoadType() const;

  virtual unsigned decodedFrameCount() const;
  virtual unsigned droppedFrameCount() const;
  virtual unsigned audioDecodedByteCount() const;
  virtual unsigned videoDecodedByteCount() const;

  virtual WebKit::WebVideoFrame* getCurrentFrame();
  virtual void putCurrentFrame(WebKit::WebVideoFrame* web_video_frame);

  
  
  
  
  virtual void WillDestroyCurrentMessageLoop();

  void Repaint();

  void OnPipelineInitialize(media::PipelineStatus status);

  void OnPipelineSeek(media::PipelineStatus status);

  void OnPipelineEnded(media::PipelineStatus status);

  void OnPipelineError(media::PipelineStatus error);

  void OnNetworkEvent(media::PipelineStatus status);

 private:
  
  
  void SetNetworkState(WebKit::WebMediaPlayer::NetworkState state);
  void SetReadyState(WebKit::WebMediaPlayer::ReadyState state);

  
  void Destroy();

  
  WebKit::WebMediaPlayerClient* GetClient();

  
  WebKit::WebMediaPlayer::NetworkState network_state_;
  WebKit::WebMediaPlayer::ReadyState ready_state_;

  
  WebKit::WebTimeRanges buffered_;

  
  
  MessageLoop* main_loop_;

  
  scoped_ptr<media::FilterCollection> filter_collection_;

  
  scoped_refptr<media::Pipeline> pipeline_;

  scoped_ptr<media::MessageLoopFactory> message_loop_factory_;

  
  
  
  
  
  
  
  
  
  
  
  
  bool paused_;
  bool seeking_;
  float playback_rate_;
  base::TimeDelta paused_time_;

  WebKit::WebMediaPlayerClient* client_;

  scoped_refptr<Proxy> proxy_;

#if WEBKIT_USING_CG
  scoped_ptr<skia::PlatformCanvas> skia_canvas_;
#endif

  DISALLOW_COPY_AND_ASSIGN(WebMediaPlayerImpl);
};

}  

#endif  
