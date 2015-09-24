// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MEDIA_STREAM_CAPTURE_INDICATOR_H_
#define CHROME_BROWSER_MEDIA_MEDIA_STREAM_CAPTURE_INDICATOR_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/status_icons/status_icon_menu_model.h"
#include "content/public/common/media_stream_request.h"

namespace content {
class WebContents;
}  

namespace gfx {
class ImageSkia;
}  

class StatusIcon;
class StatusTray;

class MediaStreamCaptureIndicator
    : public base::RefCountedThreadSafe<MediaStreamCaptureIndicator>,
      public StatusIconMenuModel::Delegate {
 public:
  MediaStreamCaptureIndicator();

  
  
  scoped_ptr<content::MediaStreamUI> RegisterMediaStream(
      content::WebContents* web_contents,
      const content::MediaStreamDevices& devices);

  
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  
  bool IsCapturingUserMedia(content::WebContents* web_contents) const;

  
  bool IsCapturingVideo(content::WebContents* web_contents) const;

  
  bool IsCapturingAudio(content::WebContents* web_contents) const;

  
  
  bool IsBeingMirrored(content::WebContents* web_contents) const;

 private:
  class UIDelegate;
  class WebContentsDeviceUsage;
  friend class WebContentsDeviceUsage;

  friend class base::RefCountedThreadSafe<MediaStreamCaptureIndicator>;
  virtual ~MediaStreamCaptureIndicator();

  

  
  
  void UnregisterWebContents(content::WebContents* web_contents);

  
  void UpdateNotificationUserInterface();

  
  
  void EnsureStatusTrayIconResources();
  void MaybeCreateStatusTrayIcon(bool audio, bool video);
  void MaybeDestroyStatusTrayIcon();

  
  void GetStatusTrayIconInfo(bool audio,
                             bool video,
                             gfx::ImageSkia* image,
                             base::string16* tool_tip);

  
  
  StatusIcon* status_icon_;

  
  gfx::ImageSkia* mic_image_;
  gfx::ImageSkia* camera_image_;

  
  
  typedef std::map<content::WebContents*, WebContentsDeviceUsage*> UsageMap;
  UsageMap usage_map_;

  
  
  
  typedef std::vector<content::WebContents*> CommandTargets;
  CommandTargets command_targets_;
};

#endif  
