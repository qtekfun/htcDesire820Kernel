// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_EVENT_TYPE_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_EVENT_TYPE_H_

namespace performance_monitor {

enum EventType {
  EVENT_UNDEFINED,

  
  EVENT_EXTENSION_INSTALL,
  EVENT_EXTENSION_UNINSTALL,
  EVENT_EXTENSION_UPDATE,
  EVENT_EXTENSION_ENABLE,
  EVENT_EXTENSION_DISABLE,

  
  EVENT_CHROME_UPDATE,

  
  
  
  EVENT_RENDERER_HANG,
  EVENT_RENDERER_CRASH,
  EVENT_RENDERER_KILLED,

  
  EVENT_UNCLEAN_EXIT,

  EVENT_NUMBER_OF_EVENTS
};

}  

#endif  
