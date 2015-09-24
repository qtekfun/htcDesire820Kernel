// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEBRTC_AUDIO_PRIVATE_WEBRTC_AUDIO_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEBRTC_AUDIO_PRIVATE_WEBRTC_AUDIO_PRIVATE_API_H_

#include "base/memory/ref_counted.h"
#include "base/system_monitor/system_monitor.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/extensions/api/webrtc_audio_private.h"
#include "content/public/browser/render_view_host.h"
#include "media/audio/audio_device_name.h"
#include "url/gurl.h"

namespace base {
class MessageLoopProxy;
}

namespace extensions {

class WebrtcAudioPrivateEventService
    : public ProfileKeyedAPI,
      public base::SystemMonitor::DevicesChangedObserver {
 public:
  explicit WebrtcAudioPrivateEventService(Profile* profile);
  virtual ~WebrtcAudioPrivateEventService();

  
  virtual void Shutdown() OVERRIDE;
  static ProfileKeyedAPIFactory<WebrtcAudioPrivateEventService>*
      GetFactoryInstance();
  static const char* service_name();

  
  virtual void OnDevicesChanged(
      base::SystemMonitor::DeviceType device_type) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<WebrtcAudioPrivateEventService>;

  void SignalEvent();

  Profile* profile_;
};

class WebrtcAudioPrivateGetSinksFunction : public ChromeAsyncExtensionFunction {
 protected:
  virtual ~WebrtcAudioPrivateGetSinksFunction() {}

 private:
  DECLARE_EXTENSION_FUNCTION("webrtcAudioPrivate.getSinks",
                             WEBRTC_AUDIO_PRIVATE_GET_SINKS);

  virtual bool RunImpl() OVERRIDE;
  void DoQuery();
  void DoneOnUIThread();
};

class WebrtcAudioPrivateTabIdFunction : public ChromeAsyncExtensionFunction {
 protected:
  virtual ~WebrtcAudioPrivateTabIdFunction() {}

 protected:
  bool DoRunImpl(int tab_id);
  virtual void OnControllerList(
      const content::RenderViewHost::AudioOutputControllerList& list) = 0;
};

class WebrtcAudioPrivateGetActiveSinkFunction
    : public WebrtcAudioPrivateTabIdFunction {
 protected:
  virtual ~WebrtcAudioPrivateGetActiveSinkFunction() {}

 private:
  DECLARE_EXTENSION_FUNCTION("webrtcAudioPrivate.getActiveSink",
                             WEBRTC_AUDIO_PRIVATE_GET_ACTIVE_SINK);

  virtual bool RunImpl() OVERRIDE;
  virtual void OnControllerList(
      const content::RenderViewHost::AudioOutputControllerList&
      controllers) OVERRIDE;
  void OnSinkId(const std::string&);
};

class WebrtcAudioPrivateSetActiveSinkFunction
    : public WebrtcAudioPrivateTabIdFunction {
 public:
  WebrtcAudioPrivateSetActiveSinkFunction();

 protected:
  virtual ~WebrtcAudioPrivateSetActiveSinkFunction();

 private:
  DECLARE_EXTENSION_FUNCTION("webrtcAudioPrivate.setActiveSink",
                             WEBRTC_AUDIO_PRIVATE_SET_ACTIVE_SINK);

  virtual bool RunImpl() OVERRIDE;
  virtual void OnControllerList(
      const content::RenderViewHost::AudioOutputControllerList&
      controllers) OVERRIDE;
  void SwitchDone();
  void DoneOnUIThread();

  
  const scoped_refptr<base::MessageLoopProxy> message_loop_;

  int tab_id_;
  std::string sink_id_;

  
  
  size_t num_remaining_sink_ids_;
};

class WebrtcAudioPrivateGetAssociatedSinkFunction
    : public ChromeAsyncExtensionFunction {
 public:
  WebrtcAudioPrivateGetAssociatedSinkFunction();

 protected:
  virtual ~WebrtcAudioPrivateGetAssociatedSinkFunction();

 private:
  DECLARE_EXTENSION_FUNCTION("webrtcAudioPrivate.getAssociatedSink",
                             WEBRTC_AUDIO_PRIVATE_GET_ASSOCIATED_SINK);

  virtual bool RunImpl() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  void GetDevicesOnDeviceThread();
  void OnGetDevicesDone();

  
  
  std::string GetRawSourceIDOnIOThread(content::ResourceContext* context,
                                       GURL security_origin,
                                       const std::string& source_id_in_origin);
  void OnGetRawSourceIDDone(const std::string& raw_source_id);

  
  
  std::string GetAssociatedSinkOnDeviceThread(const std::string& raw_source_id);
  void OnGetAssociatedSinkDone(const std::string& associated_sink_id);

  
  
  scoped_ptr<api::webrtc_audio_private::GetAssociatedSink::Params> params_;

  
  media::AudioDeviceNames source_devices_;
};

}  

#endif  
