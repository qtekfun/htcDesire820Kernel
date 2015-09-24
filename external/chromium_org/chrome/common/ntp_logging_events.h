// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NTP_LOGGING_EVENTS_H_
#define CHROME_COMMON_NTP_LOGGING_EVENTS_H_

enum NTPLoggingEventType {
  
  NTP_MOUSEOVER = 0,

  
  NTP_THUMBNAIL_ATTEMPT = 1,

  
  
  NTP_THUMBNAIL_ERROR = 2,

  
  
  NTP_FALLBACK_THUMBNAIL_REQUESTED = 3,

  
  
  NTP_FALLBACK_THUMBNAIL_USED = 4,

  
  NTP_SERVER_SIDE_SUGGESTION = 5,

  
  NTP_CLIENT_SIDE_SUGGESTION = 6,

  
  NTP_EXTERNAL_TILE = 7,

  NTP_NUM_EVENT_TYPES
};

#endif  
