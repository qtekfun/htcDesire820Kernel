// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MEDIA_CAPTURE_DEVICES_DISPATCHER_H_
#define CHROME_BROWSER_MEDIA_MEDIA_CAPTURE_DEVICES_DISPATCHER_H_

#include <deque>
#include <list>
#include <map>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "content/public/browser/media_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/common/media_stream_request.h"

class AudioStreamIndicator;
class DesktopStreamsRegistry;
class MediaStreamCaptureIndicator;
class Profile;

namespace extensions {
class Extension;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class MediaCaptureDevicesDispatcher : public content::MediaObserver,
                                      public content::NotificationObserver {
 public:
  class Observer {
   public:
    
    
    virtual void OnUpdateAudioDevices(
        const content::MediaStreamDevices& devices) {}

    
    
    virtual void OnUpdateVideoDevices(
        const content::MediaStreamDevices& devices) {}

    
    virtual void OnRequestUpdate(
        int render_process_id,
        int render_view_id,
        const content::MediaStreamDevice& device,
        const content::MediaRequestState state) {}

    
    virtual void OnCreatingAudioStream(int render_process_id,
                                       int render_view_id) {}

    virtual ~Observer() {}
  };

  static MediaCaptureDevicesDispatcher* GetInstance();

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  const content::MediaStreamDevices& GetAudioCaptureDevices();
  const content::MediaStreamDevices& GetVideoCaptureDevices();

  
  
  
  void ProcessMediaAccessRequest(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback,
      const extensions::Extension* extension);

  
  
  
  
  
  void GetDefaultDevicesForProfile(Profile* profile,
                                   bool audio,
                                   bool video,
                                   content::MediaStreamDevices* devices);

  
  
  const content::MediaStreamDevice*
  GetRequestedAudioDevice(const std::string& requested_audio_device_id);
  const content::MediaStreamDevice*
  GetRequestedVideoDevice(const std::string& requested_video_device_id);

  
  
  const content::MediaStreamDevice* GetFirstAvailableAudioDevice();
  const content::MediaStreamDevice* GetFirstAvailableVideoDevice();

  
  
  
  void DisableDeviceEnumerationForTesting();

  
  virtual void OnAudioCaptureDevicesChanged(
      const content::MediaStreamDevices& devices) OVERRIDE;
  virtual void OnVideoCaptureDevicesChanged(
      const content::MediaStreamDevices& devices) OVERRIDE;
  virtual void OnMediaRequestStateChanged(
      int render_process_id,
      int render_view_id,
      int page_request_id,
      const content::MediaStreamDevice& device,
      content::MediaRequestState state) OVERRIDE;
  virtual void OnAudioStreamPlayingChanged(
      int render_process_id,
      int render_view_id,
      int stream_id,
      bool is_playing,
      float power_dBFS,
      bool clipped) OVERRIDE;
  virtual void OnCreatingAudioStream(int render_process_id,
                                     int render_view_id) OVERRIDE;

  scoped_refptr<MediaStreamCaptureIndicator> GetMediaStreamCaptureIndicator();

  scoped_refptr<AudioStreamIndicator> GetAudioStreamIndicator();

  DesktopStreamsRegistry* GetDesktopStreamsRegistry();

  bool IsDesktopCaptureInProgress();

 private:
  friend struct DefaultSingletonTraits<MediaCaptureDevicesDispatcher>;

  struct PendingAccessRequest {
    PendingAccessRequest(const content::MediaStreamRequest& request,
                         const content::MediaResponseCallback& callback);
    ~PendingAccessRequest();

    content::MediaStreamRequest request;
    content::MediaResponseCallback callback;
  };
  typedef std::deque<PendingAccessRequest> RequestsQueue;
  typedef std::map<content::WebContents*, RequestsQueue> RequestsQueues;

  MediaCaptureDevicesDispatcher();
  virtual ~MediaCaptureDevicesDispatcher();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void ProcessDesktopCaptureAccessRequest(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback,
      const extensions::Extension* extension);
  void ProcessScreenCaptureAccessRequest(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback,
      const extensions::Extension* extension);
  void ProcessTabCaptureAccessRequest(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback,
      const extensions::Extension* extension);
  void ProcessMediaAccessRequestFromPlatformAppOrExtension(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback,
      const extensions::Extension* extension);
  void ProcessRegularMediaAccessRequest(
      content::WebContents* web_contents,
      const content::MediaStreamRequest& request,
      const content::MediaResponseCallback& callback);
  void ProcessQueuedAccessRequest(content::WebContents* web_contents);
  void OnAccessRequestResponse(content::WebContents* web_contents,
                               const content::MediaStreamDevices& devices,
                               scoped_ptr<content::MediaStreamUI> ui);

  
  void UpdateAudioDevicesOnUIThread(const content::MediaStreamDevices& devices);
  void UpdateVideoDevicesOnUIThread(const content::MediaStreamDevices& devices);
  void UpdateMediaRequestStateOnUIThread(
      int render_process_id,
      int render_view_id,
      int page_request_id,
      const content::MediaStreamDevice& device,
      content::MediaRequestState state);
  void OnCreatingAudioStreamOnUIThread(int render_process_id,
                                       int render_view_id);

  
  content::MediaStreamDevices audio_devices_;

  
  content::MediaStreamDevices video_devices_;

  
  ObserverList<Observer> observers_;

  
  
  bool devices_enumerated_;

  
  bool is_device_enumeration_disabled_;

  RequestsQueues pending_requests_;

  scoped_refptr<MediaStreamCaptureIndicator> media_stream_capture_indicator_;

  scoped_refptr<AudioStreamIndicator> audio_stream_indicator_;

  scoped_ptr<DesktopStreamsRegistry> desktop_streams_registry_;

  content::NotificationRegistrar notifications_registrar_;

  
  
  
  struct DesktopCaptureSession {
    int render_process_id;
    int render_view_id;
    int page_request_id;
  };
  typedef std::list<DesktopCaptureSession> DesktopCaptureSessions;
  DesktopCaptureSessions desktop_capture_sessions_;

  DISALLOW_COPY_AND_ASSIGN(MediaCaptureDevicesDispatcher);
};

#endif  