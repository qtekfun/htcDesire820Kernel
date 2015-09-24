// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_CHROME_MIDI_PERMISSION_CONTEXT_H_
#define CHROME_BROWSER_MEDIA_CHROME_MIDI_PERMISSION_CONTEXT_H_

#include "base/memory/scoped_ptr.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/browser_context.h"

class GURL;
class PermissionQueueController;
class PermissionRequestID;
class Profile;

class ChromeMIDIPermissionContext : public BrowserContextKeyedService {
 public:
  explicit ChromeMIDIPermissionContext(Profile* profile);
  virtual ~ChromeMIDIPermissionContext();

  
  virtual void Shutdown() OVERRIDE;

  
  void RequestMIDISysExPermission(
      int render_process_id,
      int render_view_id,
      int bridge_id,
      const GURL& requesting_frame,
      const content::BrowserContext::MIDISysExPermissionCallback& callback);

  
  void CancelMIDISysExPermissionRequest(int render_process_id,
                                        int render_view_id,
                                        int bridge_id,
                                        const GURL& requesting_frame);

 private:
  
  
  
  void DecidePermission(
      const PermissionRequestID& id,
      const GURL& requesting_frame,
      const GURL& embedder,
      const content::BrowserContext::MIDISysExPermissionCallback& callback);

  
  void PermissionDecided(
      const PermissionRequestID& id,
      const GURL& requesting_frame,
      const GURL& embedder,
      const content::BrowserContext::MIDISysExPermissionCallback& callback,
      bool allowed);

  
  
  void NotifyPermissionSet(
      const PermissionRequestID& id,
      const GURL& requesting_frame,
      const content::BrowserContext::MIDISysExPermissionCallback& callback,
      bool allowed);

  
  PermissionQueueController* GetQueueController();

  
  void CancelPendingInfoBarRequest(const PermissionRequestID& id);

  Profile* const profile_;
  bool shutting_down_;
  scoped_ptr<PermissionQueueController> permission_queue_controller_;

  DISALLOW_COPY_AND_ASSIGN(ChromeMIDIPermissionContext);
};

#endif  
