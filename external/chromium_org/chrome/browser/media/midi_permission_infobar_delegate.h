// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MIDI_PERMISSION_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_MEDIA_MIDI_PERMISSION_INFOBAR_DELEGATE_H_

#include <string>

#include "chrome/browser/content_settings/permission_request_id.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "url/gurl.h"

class PermissionQueueController;
class InfoBarService;


class MIDIPermissionInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  static InfoBar* Create(InfoBarService* infobar_service,
                         PermissionQueueController* controller,
                         const PermissionRequestID& id,
                         const GURL& requesting_frame,
                         const std::string& display_languages);

 private:
  MIDIPermissionInfoBarDelegate(PermissionQueueController* controller,
                                const PermissionRequestID& id,
                                const GURL& requesting_frame,
                                int contents_unique_id,
                                const std::string& display_languages);
  virtual ~MIDIPermissionInfoBarDelegate();

  
  virtual void InfoBarDismissed() OVERRIDE;
  virtual int GetIconID() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual bool ShouldExpireInternal(
      const content::LoadCommittedDetails& details) const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;

  
  void SetPermission(bool update_content_setting, bool allowed);

 private:
  PermissionQueueController* controller_;
  const PermissionRequestID id_;
  GURL requesting_frame_;
  int contents_unique_id_;
  std::string display_languages_;

  DISALLOW_COPY_AND_ASSIGN(MIDIPermissionInfoBarDelegate);
};

#endif  
