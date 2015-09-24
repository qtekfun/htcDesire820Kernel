// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_CONTENT_SETTINGS_OBSERVER_H_
#define CHROME_RENDERER_CONTENT_SETTINGS_OBSERVER_H_

#include <map>
#include <set>

#include "chrome/common/content_settings.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/public/renderer/render_view_observer_tracker.h"
#include "extensions/common/permissions/api_permission.h"
#include "third_party/WebKit/public/web/WebPermissionClient.h"

class GURL;

namespace blink {
class WebFrame;
class WebSecurityOrigin;
class WebURL;
}

namespace extensions {
class Dispatcher;
class Extension;
}

class ContentSettingsObserver
    : public content::RenderViewObserver,
      public content::RenderViewObserverTracker<ContentSettingsObserver>,
      public blink::WebPermissionClient {
 public:
  ContentSettingsObserver(content::RenderView* render_view,
                          extensions::Dispatcher* extension_dispatcher);
  virtual ~ContentSettingsObserver();

  
  
  
  void SetContentSettingRules(
      const RendererContentSettingRules* content_setting_rules);

  bool IsPluginTemporarilyAllowed(const std::string& identifier);

  
  void DidBlockContentType(ContentSettingsType settings_type);

  
  virtual bool allowDatabase(blink::WebFrame* frame,
                             const blink::WebString& name,
                             const blink::WebString& display_name,
                             unsigned long estimated_size);
  virtual bool allowFileSystem(blink::WebFrame* frame);
  virtual bool allowImage(blink::WebFrame* frame,
                          bool enabled_per_settings,
                          const blink::WebURL& image_url);
  virtual bool allowIndexedDB(blink::WebFrame* frame,
                              const blink::WebString& name,
                              const blink::WebSecurityOrigin& origin);
  virtual bool allowPlugins(blink::WebFrame* frame,
                            bool enabled_per_settings);
  virtual bool allowScript(blink::WebFrame* frame,
                           bool enabled_per_settings);
  virtual bool allowScriptFromSource(blink::WebFrame* frame,
                                     bool enabled_per_settings,
                                     const blink::WebURL& script_url);
  virtual bool allowStorage(blink::WebFrame* frame, bool local);
  virtual bool allowReadFromClipboard(blink::WebFrame* frame,
                                      bool default_value);
  virtual bool allowWriteToClipboard(blink::WebFrame* frame,
                                     bool default_value);
  virtual bool allowWebComponents(blink::WebFrame* frame, bool);
  virtual bool allowMutationEvents(blink::WebFrame* frame,
                                   bool default_value);
  virtual bool allowPushState(blink::WebFrame* frame);
  virtual bool allowWebGLDebugRendererInfo(blink::WebFrame* frame);
  virtual void didNotAllowPlugins(blink::WebFrame* frame);
  virtual void didNotAllowScript(blink::WebFrame* frame);
  virtual bool allowDisplayingInsecureContent(
      blink::WebFrame* frame,
      bool allowed_per_settings,
      const blink::WebSecurityOrigin& context,
      const blink::WebURL& url);
  virtual bool allowRunningInsecureContent(
      blink::WebFrame* frame,
      bool allowed_per_settings,
      const blink::WebSecurityOrigin& context,
      const blink::WebURL& url);

  
  bool AreNPAPIPluginsBlocked() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(ContentSettingsObserverTest, WhitelistedSchemes);
  FRIEND_TEST_ALL_PREFIXES(ChromeRenderViewTest,
                           ContentSettingsInterstitialPages);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidCommitProvisionalLoad(blink::WebFrame* frame,
                                        bool is_new_navigation) OVERRIDE;

  
  void OnLoadBlockedPlugins(const std::string& identifier);
  void OnSetAsInterstitial();
  void OnNPAPINotSupported();
  void OnSetAllowDisplayingInsecureContent(bool allow);
  void OnSetAllowRunningInsecureContent(bool allow);

  
  void ClearBlockedContentSettings();

  
  
  const extensions::Extension* GetExtension(
      const blink::WebSecurityOrigin& origin) const;

  
  
  static bool IsWhitelistedForContentSettings(blink::WebFrame* frame);
  static bool IsWhitelistedForContentSettings(
      const blink::WebSecurityOrigin& origin,
      const GURL& document_url);

  
  extensions::Dispatcher* extension_dispatcher_;

  
  bool allow_displaying_insecure_content_;
  bool allow_running_insecure_content_;

  
  
  
  
  const RendererContentSettingRules* content_setting_rules_;

  
  bool content_blocked_[CONTENT_SETTINGS_NUM_TYPES];

  
  typedef std::pair<GURL, bool> StoragePermissionsKey;
  std::map<StoragePermissionsKey, bool> cached_storage_permissions_;

  
  std::map<blink::WebFrame*, bool> cached_script_permissions_;

  std::set<std::string> temporarily_allowed_plugins_;
  bool is_interstitial_page_;
  bool npapi_plugins_blocked_;

  DISALLOW_COPY_AND_ASSIGN(ContentSettingsObserver);
};

#endif  
