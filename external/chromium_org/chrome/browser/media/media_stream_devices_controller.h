// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MEDIA_STREAM_DEVICES_CONTROLLER_H_
#define CHROME_BROWSER_MEDIA_MEDIA_STREAM_DEVICES_CONTROLLER_H_

#include <map>
#include <string>

#include "content/public/browser/web_contents_delegate.h"

class Profile;
class TabSpecificContentSettings;

namespace content {
class WebContents;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class MediaStreamDevicesController {
 public:
  
  enum Permission {
    MEDIA_NONE,
    MEDIA_ALLOWED,
    MEDIA_BLOCKED_BY_POLICY,
    MEDIA_BLOCKED_BY_USER_SETTING,
    MEDIA_BLOCKED_BY_USER,
  };

  struct MediaStreamTypeSettings {
    MediaStreamTypeSettings(Permission permission,
                            const std::string& requested_device_id);
    MediaStreamTypeSettings();
    ~MediaStreamTypeSettings();

    Permission permission;
    std::string requested_device_id;
  };

  typedef std::map<content::MediaStreamType, MediaStreamTypeSettings>
      MediaStreamTypeSettingsMap;

  MediaStreamDevicesController(content::WebContents* web_contents,
                               const content::MediaStreamRequest& request,
                               const content::MediaResponseCallback& callback);

  virtual ~MediaStreamDevicesController();

  
  
  

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  bool DismissInfoBarAndTakeActionOnSettings();

  
  bool HasAudio() const;
  bool HasVideo() const;
  const std::string& GetSecurityOriginSpec() const;
  void Accept(bool update_content_setting);
  void Deny(bool update_content_setting);

 private:
  enum DevicePolicy {
    POLICY_NOT_SET,
    ALWAYS_DENY,
    ALWAYS_ALLOW,
  };

  
  
  DevicePolicy GetDevicePolicy(const char* policy_name,
                               const char* whitelist_policy_name) const;

  
  
  bool IsRequestAllowedByDefault() const;

  
  
  
  
  
  int FilterBlockedByDefaultDevices();

  
  
  bool IsDefaultMediaAccessBlocked() const;

  
  bool IsSchemeSecure() const;

  
  
  bool ShouldAlwaysAllowOrigin() const;

  
  
  void SetPermission(bool allowed) const;

  
  
  void NotifyUIRequestAccepted() const;

  
  
  void NotifyUIRequestDenied();

  
  
  
  
  
  
  bool IsDeviceAudioCaptureRequestedAndAllowed() const;
  bool IsDeviceVideoCaptureRequestedAndAllowed() const;

  content::WebContents* web_contents_;

  
  Profile* profile_;

  
  
  
  
  
  TabSpecificContentSettings* content_settings_;

  
  const content::MediaStreamRequest request_;

  
  
  content::MediaResponseCallback callback_;

  
  
  
  
  
  MediaStreamTypeSettingsMap request_permissions_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamDevicesController);
};

#endif  
