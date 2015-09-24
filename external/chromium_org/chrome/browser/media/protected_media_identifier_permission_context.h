// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_PROTECTED_MEDIA_IDENTIFIER_PERMISSION_CONTEXT_H_
#define CHROME_BROWSER_MEDIA_PROTECTED_MEDIA_IDENTIFIER_PERMISSION_CONTEXT_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/content_settings/permission_queue_controller.h"

class PermissionRequestID;
class Profile;

namespace content {
class RenderViewHost;
}

class ProtectedMediaIdentifierPermissionContext
    : public base::RefCountedThreadSafe<
        ProtectedMediaIdentifierPermissionContext> {
 public:
  explicit ProtectedMediaIdentifierPermissionContext(Profile* profile);

  void RequestProtectedMediaIdentifierPermission(
      int render_process_id,
      int render_view_id,
      const GURL& requesting_frame,
      const base::Callback<void(bool)>& callback);
  void CancelProtectedMediaIdentifierPermissionRequest(
      int render_process_id,
      int render_view_id,
      const GURL& requesting_frame);

  
  void ShutdownOnUIThread();

 private:
  friend class base::RefCountedThreadSafe<
      ProtectedMediaIdentifierPermissionContext>;
  ~ProtectedMediaIdentifierPermissionContext();

  Profile* profile() const { return profile_; }

  
  
  PermissionQueueController* QueueController();

  
  
  
  void NotifyPermissionSet(const PermissionRequestID& id,
                           const GURL& requesting_frame,
                           const base::Callback<void(bool)>& callback,
                           bool allowed);

  
  
  
  
  void DecidePermission(const PermissionRequestID& id,
                        const GURL& requesting_frame,
                        const GURL& embedder,
                        content::RenderViewHost* rvh,
                        const base::Callback<void(bool)>& callback);

  
  
  
  
  void PermissionDecided(const PermissionRequestID& id,
                         const GURL& requesting_frame,
                         const GURL& embedder,
                         const base::Callback<void(bool)>& callback,
                         bool allowed);

  
  
  PermissionQueueController* CreateQueueController();

  
  void CancelPendingInfoBarRequest(const PermissionRequestID& id);

  
  Profile* const profile_;
  bool shutting_down_;
  scoped_ptr<PermissionQueueController> permission_queue_controller_;

  DISALLOW_COPY_AND_ASSIGN(ProtectedMediaIdentifierPermissionContext);
};

#endif  
