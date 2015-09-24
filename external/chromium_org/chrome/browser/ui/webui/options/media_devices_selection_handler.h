// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_MEDIA_DEVICES_SELECTION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_MEDIA_DEVICES_SELECTION_HANDLER_H_

#include "chrome/browser/media/media_capture_devices_dispatcher.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "content/public/browser/web_contents.h"
#include "grit/generated_resources.h"

namespace options {

class MediaDevicesSelectionHandler
    : public MediaCaptureDevicesDispatcher::Observer,
      public OptionsPageUIHandler {
 public:
  MediaDevicesSelectionHandler();
  virtual ~MediaDevicesSelectionHandler();

  
  virtual void GetLocalizedValues(base::DictionaryValue* values) OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnUpdateAudioDevices(
      const content::MediaStreamDevices& devices) OVERRIDE;
  virtual void OnUpdateVideoDevices(
      const content::MediaStreamDevices& devices) OVERRIDE;

 private:
  enum DeviceType {
    AUDIO,
    VIDEO,
  };

  
  
  
  void SetDefaultCaptureDevice(const base::ListValue* args);

  
  void UpdateDevicesMenuForType(DeviceType type);
  void UpdateDevicesMenu(DeviceType type,
                         const content::MediaStreamDevices& devices);

  DISALLOW_COPY_AND_ASSIGN(MediaDevicesSelectionHandler);
};

}  

#endif  
