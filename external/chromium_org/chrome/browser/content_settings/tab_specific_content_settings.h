// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CONTENT_SETTINGS_TAB_SPECIFIC_CONTENT_SETTINGS_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_TAB_SPECIFIC_CONTENT_SETTINGS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/content_settings/content_settings_usages_state.h"
#include "chrome/browser/content_settings/local_shared_objects_container.h"
#include "chrome/browser/media/media_stream_devices_controller.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "chrome/common/custom_handlers/protocol_handler.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/common/media_stream_request.h"
#include "net/cookies/canonical_cookie.h"

class CookiesTreeModel;
class Profile;

namespace content {
class RenderViewHost;
}

namespace net {
class CookieOptions;
}

class TabSpecificContentSettings
    : public content::WebContentsObserver,
      public content::NotificationObserver,
      public content::WebContentsUserData<TabSpecificContentSettings> {
 public:
  enum MicrophoneCameraState {
    MICROPHONE_CAMERA_NOT_ACCESSED = 0,
    MICROPHONE_ACCESSED,
    CAMERA_ACCESSED,
    MICROPHONE_CAMERA_ACCESSED,
    MICROPHONE_BLOCKED,
    CAMERA_BLOCKED,
    MICROPHONE_CAMERA_BLOCKED,
  };

  
  
  
  class SiteDataObserver {
   public:
    explicit SiteDataObserver(
        TabSpecificContentSettings* tab_specific_content_settings);
    virtual ~SiteDataObserver();

    
    virtual void OnSiteDataAccessed() = 0;

    TabSpecificContentSettings* tab_specific_content_settings() {
      return tab_specific_content_settings_;
    }

    
    
    void ContentSettingsDestroyed();

   private:
    TabSpecificContentSettings* tab_specific_content_settings_;

    DISALLOW_COPY_AND_ASSIGN(SiteDataObserver);
  };

  virtual ~TabSpecificContentSettings();

  
  static TabSpecificContentSettings* Get(int render_process_id,
                                         int render_view_id);

  
  
  
  
  
  static void CookiesRead(int render_process_id,
                          int render_view_id,
                          const GURL& url,
                          const GURL& first_party_url,
                          const net::CookieList& cookie_list,
                          bool blocked_by_policy);

  
  
  
  
  static void CookieChanged(int render_process_id,
                            int render_view_id,
                            const GURL& url,
                            const GURL& first_party_url,
                            const std::string& cookie_line,
                            const net::CookieOptions& options,
                            bool blocked_by_policy);

  
  
  
  
  static void WebDatabaseAccessed(int render_process_id,
                                  int render_view_id,
                                  const GURL& url,
                                  const base::string16& name,
                                  const base::string16& display_name,
                                  bool blocked_by_policy);

  
  
  
  
  static void DOMStorageAccessed(int render_process_id,
                                 int render_view_id,
                                 const GURL& url,
                                 bool local,
                                 bool blocked_by_policy);

  
  
  
  
  static void IndexedDBAccessed(int render_process_id,
                                int render_view_id,
                                const GURL& url,
                                const base::string16& description,
                                bool blocked_by_policy);

  
  
  
  
  static void FileSystemAccessed(int render_process_id,
                                 int render_view_id,
                                 const GURL& url,
                                 bool blocked_by_policy);

  
  
  void ClearBlockedContentSettingsExceptForCookies();

  
  void ClearCookieSpecificContentSettings();

  
  void ClearGeolocationContentSettings();

  
  void ClearMIDIContentSettings();

  
  void SetPopupsBlocked(bool blocked);

  
  void SetDownloadsBlocked(bool blocked);

  
  void GeolocationDidNavigate(
      const content::LoadCommittedDetails& details);

  
  void MIDIDidNavigate(const content::LoadCommittedDetails& details);

  
  
  bool IsContentBlocked(ContentSettingsType content_type) const;

  
  bool IsBlockageIndicated(ContentSettingsType content_type) const;

  void SetBlockageHasBeenIndicated(ContentSettingsType content_type);

  
  
  bool IsContentAllowed(ContentSettingsType content_type) const;

  const GURL& media_stream_access_origin() const {
    return media_stream_access_origin_;
  }

  const std::string& media_stream_requested_audio_device() const {
    return media_stream_requested_audio_device_;
  }

  const std::string& media_stream_requested_video_device() const {
    return media_stream_requested_video_device_;
  }

  
  MicrophoneCameraState GetMicrophoneCameraState() const;

  
  
  const ContentSettingsUsagesState& geolocation_usages_state() const {
    return geolocation_usages_state_;
  }

  
  
  const ContentSettingsUsagesState& midi_usages_state() const {
    return midi_usages_state_;
  }

  
  void set_pending_protocol_handler(const ProtocolHandler& handler) {
    pending_protocol_handler_ = handler;
  }

  const ProtocolHandler& pending_protocol_handler() const {
    return pending_protocol_handler_;
  }

  void ClearPendingProtocolHandler() {
    pending_protocol_handler_ = ProtocolHandler::EmptyProtocolHandler();
  }

  
  
  void set_previous_protocol_handler(const ProtocolHandler& handler) {
    previous_protocol_handler_ = handler;
  }

  const ProtocolHandler& previous_protocol_handler() const {
    return previous_protocol_handler_;
  }

  
  
  void set_pending_protocol_handler_setting(ContentSetting setting) {
    pending_protocol_handler_setting_ = setting;
  }

  ContentSetting pending_protocol_handler_setting() const {
    return pending_protocol_handler_setting_;
  }


  
  
  const LocalSharedObjectsContainer& allowed_local_shared_objects() const {
    return allowed_local_shared_objects_;
  }

  
  
  const LocalSharedObjectsContainer& blocked_local_shared_objects() const {
    return blocked_local_shared_objects_;
  }

  bool load_plugins_link_enabled() { return load_plugins_link_enabled_; }
  void set_load_plugins_link_enabled(bool enabled) {
    load_plugins_link_enabled_ = enabled;
  }

  
  
  void SetPepperBrokerAllowed(bool allowed);

  
  virtual void RenderViewForInterstitialPageCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void AppCacheAccessed(const GURL& manifest_url,
                                bool blocked_by_policy) OVERRIDE;

  
  void OnContentBlocked(ContentSettingsType type);
  void OnContentAllowed(ContentSettingsType type);

  
  
  void OnCookiesRead(const GURL& url,
                     const GURL& first_party_url,
                     const net::CookieList& cookie_list,
                     bool blocked_by_policy);
  void OnCookieChanged(const GURL& url,
                       const GURL& first_party_url,
                       const std::string& cookie_line,
                       const net::CookieOptions& options,
                       bool blocked_by_policy);
  void OnFileSystemAccessed(const GURL& url,
                            bool blocked_by_policy);
  void OnIndexedDBAccessed(const GURL& url,
                           const base::string16& description,
                           bool blocked_by_policy);
  void OnLocalStorageAccessed(const GURL& url,
                              bool local,
                              bool blocked_by_policy);
  void OnWebDatabaseAccessed(const GURL& url,
                             const base::string16& name,
                             const base::string16& display_name,
                             bool blocked_by_policy);
  void OnGeolocationPermissionSet(const GURL& requesting_frame,
                                  bool allowed);
#if defined(OS_ANDROID)
  void OnProtectedMediaIdentifierPermissionSet(const GURL& requesting_frame,
                                               bool allowed);
#endif

  
  
  
  void OnMediaStreamPermissionSet(
      const GURL& request_origin,
      const MediaStreamDevicesController::MediaStreamTypeSettingsMap&
          request_permissions);

  
  void OnMIDISysExAccessed(const GURL& reqesting_origin);
  void OnMIDISysExAccessBlocked(const GURL& requesting_origin);

  
  
  void AddSiteDataObserver(SiteDataObserver* observer);

  
  void RemoveSiteDataObserver(SiteDataObserver* observer);

 private:
  explicit TabSpecificContentSettings(content::WebContents* tab);
  friend class content::WebContentsUserData<TabSpecificContentSettings>;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void NotifySiteDataObservers();

  
  ObserverList<SiteDataObserver> observer_list_;

  
  bool content_blocked_[CONTENT_SETTINGS_NUM_TYPES];

  
  bool content_blockage_indicated_to_user_[CONTENT_SETTINGS_NUM_TYPES];

  
  bool content_allowed_[CONTENT_SETTINGS_NUM_TYPES];

  
  Profile* profile_;

  
  LocalSharedObjectsContainer allowed_local_shared_objects_;
  LocalSharedObjectsContainer blocked_local_shared_objects_;

  
  ContentSettingsUsagesState geolocation_usages_state_;

  
  ContentSettingsUsagesState midi_usages_state_;

  
  
  
  
  ProtocolHandler pending_protocol_handler_;

  
  
  
  ProtocolHandler previous_protocol_handler_;

  
  
  ContentSetting pending_protocol_handler_setting_;

  
  bool load_plugins_link_enabled_;

  content::NotificationRegistrar registrar_;

  
  
  
  GURL media_stream_access_origin_;

  
  
  std::string media_stream_requested_audio_device_;
  std::string media_stream_requested_video_device_;

  DISALLOW_COPY_AND_ASSIGN(TabSpecificContentSettings);
};

#endif  
