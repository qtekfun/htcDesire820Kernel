// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NTP_USER_DATA_LOGGER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NTP_USER_DATA_LOGGER_H_

#include "chrome/common/ntp_logging_events.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class NTPUserDataLogger
    : public content::WebContentsObserver,
      public content::WebContentsUserData<NTPUserDataLogger> {
 public:
  virtual ~NTPUserDataLogger();

  static NTPUserDataLogger* GetOrCreateFromWebContents(
      content::WebContents* content);

  
  
  
  void EmitThumbnailErrorRate();

  
  
  
  void EmitNtpStatistics();

  
  
  void LogEvent(NTPLoggingEventType event);

  
  void LogImpression(int position, const base::string16& provider);

  
  virtual void NavigationEntryCommitted(
      const content::LoadCommittedDetails& load_details) OVERRIDE;

 protected:
  explicit NTPUserDataLogger(content::WebContents* contents);

  
  virtual size_t GetPercentError(size_t errors, size_t events) const;

 private:
  friend class content::WebContentsUserData<NTPUserDataLogger>;

  
  size_t number_of_mouseovers_;

  
  
  size_t number_of_thumbnail_attempts_;

  
  
  
  size_t number_of_thumbnail_errors_;

  
  
  size_t number_of_fallback_thumbnails_requested_;

  
  
  size_t number_of_fallback_thumbnails_used_;

  
  
  size_t number_of_external_tiles_;

  
  
  bool server_side_suggestions_;

  
  GURL ntp_url_;

  DISALLOW_COPY_AND_ASSIGN(NTPUserDataLogger);
};

#endif  
