// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_SHOW_DESKTOP_NOTIFICATION_PARAMS_H_
#define CONTENT_PUBLIC_COMMON_SHOW_DESKTOP_NOTIFICATION_PARAMS_H_

#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT ShowDesktopNotificationHostMsgParams {
  ShowDesktopNotificationHostMsgParams();
  ~ShowDesktopNotificationHostMsgParams();

  
  GURL origin;

  
  GURL icon_url;
  base::string16 title;
  base::string16 body;

  
  blink::WebTextDirection direction;

  
  
  base::string16 replace_id;

  
  int notification_id;
};

}  

#endif  
