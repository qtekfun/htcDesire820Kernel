// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GUESTVIEW_GUESTVIEW_H_
#define CHROME_BROWSER_GUESTVIEW_GUESTVIEW_H_

#include <queue>

#include "base/values.h"
#include "content/public/browser/browser_plugin_guest_delegate.h"
#include "content/public/browser/web_contents.h"

class AdViewGuest;
class WebViewGuest;
struct RendererContentSettingRules;

class GuestView : public content::BrowserPluginGuestDelegate {
 public:
  enum Type {
    WEBVIEW,
    ADVIEW,
    UNKNOWN
  };

  class Event {
   public:
     Event(const std::string& name, scoped_ptr<DictionaryValue> args);
     ~Event();

    const std::string& name() const { return name_; }

    scoped_ptr<DictionaryValue> GetArguments();

   private:
    const std::string name_;
    scoped_ptr<DictionaryValue> args_;
  };

  static Type GetViewTypeFromString(const std::string& api_type);

  static GuestView* Create(content::WebContents* guest_web_contents,
                           const std::string& extension_id,
                           Type view_type);

  static GuestView* FromWebContents(content::WebContents* web_contents);

  static GuestView* From(int embedder_process_id, int instance_id);

  
  
  
  
  
  
  
  static bool GetGuestPartitionConfigForSite(const GURL& site,
                                             std::string* partition_domain,
                                             std::string* partition_name,
                                             bool* in_memory);

  
  static void GetDefaultContentSettingRules(
      RendererContentSettingRules* rules, bool incognito);

  virtual void Attach(content::WebContents* embedder_web_contents,
                      const base::DictionaryValue& args);

  content::WebContents* embedder_web_contents() const {
    return embedder_web_contents_;
  }

  
  content::WebContents* guest_web_contents() const {
    return guest_web_contents_;
  }

  virtual Type GetViewType() const;

  
  virtual WebViewGuest* AsWebView() = 0;

  
  virtual AdViewGuest* AsAdView() = 0;

  
  bool attached() const { return !!embedder_web_contents_; }

  
  int view_instance_id() const { return view_instance_id_; }

  
  int guest_instance_id() const { return guest_instance_id_; }

  
  const std::string& extension_id() const { return extension_id_; }

  
  content::BrowserContext* browser_context() const { return browser_context_; }

  
  int embedder_render_process_id() const { return embedder_render_process_id_; }

 protected:
  GuestView(content::WebContents* guest_web_contents,
            const std::string& extension_id);
  virtual ~GuestView();

  
  void DispatchEvent(Event* event);

 private:
  void SendQueuedEvents();

  content::WebContents* const guest_web_contents_;
  content::WebContents* embedder_web_contents_;
  const std::string extension_id_;
  int embedder_render_process_id_;
  content::BrowserContext* const browser_context_;
  
  
  const int guest_instance_id_;
  
  
  int view_instance_id_;

  
  
  std::queue<Event*> pending_events_;

  DISALLOW_COPY_AND_ASSIGN(GuestView);
};

#endif  
