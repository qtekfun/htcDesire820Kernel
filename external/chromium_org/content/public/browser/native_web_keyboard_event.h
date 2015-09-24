// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NATIVE_WEB_KEYBOARD_EVENT_H_
#define CONTENT_PUBLIC_BROWSER_NATIVE_WEB_KEYBOARD_EVENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "build/build_config.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "ui/events/event_constants.h"
#include "ui/gfx/native_widget_types.h"

namespace content {

struct CONTENT_EXPORT NativeWebKeyboardEvent :
  NON_EXPORTED_BASE(public blink::WebKeyboardEvent) {
  NativeWebKeyboardEvent();

  explicit NativeWebKeyboardEvent(gfx::NativeEvent native_event);
#if defined(USE_AURA)
  NativeWebKeyboardEvent(ui::EventType type,
                         bool is_char,
                         wchar_t character,
                         int state,
                         double time_stamp_seconds);
#elif defined(OS_MACOSX) || defined(TOOLKIT_GTK)
  
  
  
  
  NativeWebKeyboardEvent(wchar_t character,
                         int state,
                         double time_stamp_seconds);
#elif defined(OS_ANDROID)
  NativeWebKeyboardEvent(blink::WebInputEvent::Type type,
                         int modifiers,
                         double time_secs,
                         int keycode,
                         int unicode_character,
                         bool is_system_key);
  
  NativeWebKeyboardEvent(jobject android_key_event,
                         blink::WebInputEvent::Type type,
                         int modifiers,
                         double time_secs,
                         int keycode,
                         int unicode_character,
                         bool is_system_key);
#endif

  NativeWebKeyboardEvent(const NativeWebKeyboardEvent& event);
  ~NativeWebKeyboardEvent();

  NativeWebKeyboardEvent& operator=(const NativeWebKeyboardEvent& event);

  gfx::NativeEvent os_event;

  
  
  
  
  
  bool skip_in_browser;

#if defined(TOOLKIT_GTK)
  
  
  
  bool match_edit_command;
#endif
};

CONTENT_EXPORT int GetModifiersFromNativeWebKeyboardEvent(
    const NativeWebKeyboardEvent& event);

}  

#endif  
