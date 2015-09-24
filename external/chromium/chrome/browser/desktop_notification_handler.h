// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DESKTOP_NOTIFICATION_HANDLER_H_
#define CHROME_BROWSER_DESKTOP_NOTIFICATION_HANDLER_H_
#pragma once

#include "content/browser/renderer_host/render_view_host_observer.h"

class GURL;
struct DesktopNotificationHostMsg_Show_Params;

class DesktopNotificationHandler : public RenderViewHostObserver {
 public:
  explicit DesktopNotificationHandler(RenderViewHost* render_view_host);
  virtual ~DesktopNotificationHandler();

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  void OnShow(const DesktopNotificationHostMsg_Show_Params& params);
  void OnCancel(int notification_id);
  void OnRequestPermission(const GURL& origin, int callback_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopNotificationHandler);
};

#endif  

