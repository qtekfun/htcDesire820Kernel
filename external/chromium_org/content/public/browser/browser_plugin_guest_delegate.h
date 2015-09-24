// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_PLUGIN_GUEST_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_PLUGIN_GUEST_DELEGATE_H_

#include "base/callback_forward.h"
#include "base/process/kill.h"
#include "base/strings/string16.h"
#include "base/values.h"
#include "content/common/content_export.h"
#include "content/public/common/browser_plugin_permission_type.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

namespace content {

struct NativeWebKeyboardEvent;

class CONTENT_EXPORT BrowserPluginGuestDelegate {
 public:
  virtual ~BrowserPluginGuestDelegate() {}

  
  virtual void AddMessageToConsole(int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) {}

  
  
  virtual void Close() {}

  
  virtual void DidAttach() {}

  
  
  
  virtual void GuestProcessGone(base::TerminationStatus status) {}

  
  virtual void EmbedderDestroyed() {}

  virtual bool HandleKeyboardEvent(const NativeWebKeyboardEvent& event);

  virtual bool IsDragAndDropEnabled();

  
  virtual bool IsOverridingUserAgent() const;

  
  
  virtual void LoadAbort(bool is_top_level,
                         const GURL& url,
                         const std::string& error_type) {}

  
  
  virtual void LoadProgressed(double progress) {}

  
  virtual void RendererResponsive() {}

  
  virtual void RendererUnresponsive() {}

  typedef base::Callback<void(bool ,
                              const std::string& )>
      PermissionResponseCallback;

  
  
  
  
  virtual bool RequestPermission(
      BrowserPluginPermissionType permission_type,
      const base::DictionaryValue& request_info,
      const PermissionResponseCallback& callback,
      bool allowed_by_default);

  
  virtual GURL ResolveURL(const std::string& src);

  
  virtual void SizeChanged(const gfx::Size& old_size,
                           const gfx::Size& new_size) {}
};

}  

#endif  
