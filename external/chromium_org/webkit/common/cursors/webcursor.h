// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_CURSORS_WEBCURSOR_H_
#define WEBKIT_COMMON_CURSORS_WEBCURSOR_H_

#include "base/basictypes.h"
#include "third_party/WebKit/public/platform/WebCursorInfo.h"
#include "ui/gfx/display.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"
#include "webkit/common/webkit_common_export.h"

#include <vector>

#if defined(USE_AURA)
#include "ui/base/cursor/cursor.h"
#endif

#if defined(OS_WIN)
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HICON__* HICON;
typedef HICON HCURSOR;
#elif defined(TOOLKIT_GTK)
typedef struct _GdkCursor GdkCursor;
#elif defined(OS_MACOSX)
#ifdef __OBJC__
@class NSCursor;
#else
class NSCursor;
#endif
#endif

class Pickle;
class PickleIterator;

class WEBKIT_COMMON_EXPORT WebCursor {
 public:
  struct CursorInfo {
    explicit CursorInfo(blink::WebCursorInfo::Type cursor_type)
        : type(cursor_type),
          image_scale_factor(1) {
#if defined(OS_WIN)
      external_handle = NULL;
#endif
    }

    CursorInfo()
        : type(blink::WebCursorInfo::TypePointer),
          image_scale_factor(1) {
#if defined(OS_WIN)
      external_handle = NULL;
#endif
    }

    blink::WebCursorInfo::Type type;
    gfx::Point hotspot;
    float image_scale_factor;
    SkBitmap custom_image;
#if defined(OS_WIN)
    HCURSOR external_handle;
#endif
  };

  WebCursor();
  explicit WebCursor(const CursorInfo& cursor_info);
  ~WebCursor();

  
  WebCursor(const WebCursor& other);
  const WebCursor& operator=(const WebCursor& other);

  
  void InitFromCursorInfo(const CursorInfo& cursor_info);
  void GetCursorInfo(CursorInfo* cursor_info) const;

  
  bool Deserialize(PickleIterator* iter);
  bool Serialize(Pickle* pickle) const;

  
  
  bool IsCustom() const;

  
  
  
  bool IsEqual(const WebCursor& other) const;

  
  gfx::NativeCursor GetNativeCursor();

#if defined(OS_WIN)
  
  
  
  void InitFromExternalCursor(HCURSOR handle);
#endif

#if defined(USE_AURA)
  const ui::PlatformCursor GetPlatformCursor();

  
  void SetDisplayInfo(const gfx::Display& display);

#elif defined(OS_WIN)
  
  
  
  HCURSOR GetCursor(HINSTANCE module_handle);

#elif defined(TOOLKIT_GTK)
  
  
  
  
  int GetCursorType() const;

  
  
  GdkCursor* GetCustomCursor();
#elif defined(OS_MACOSX)
  
  void InitFromNSCursor(NSCursor* cursor);
#endif

 private:
  
  void Copy(const WebCursor& other);

  
  void Clear();

  
  void InitPlatformData();

  
  bool SerializePlatformData(Pickle* pickle) const;
  bool DeserializePlatformData(PickleIterator* iter);

  
  
  bool IsPlatformDataEqual(const WebCursor& other) const ;

  
  void CopyPlatformData(const WebCursor& other);

  
  void CleanupPlatformData();

  void SetCustomData(const SkBitmap& image);
  void ImageFromCustomData(SkBitmap* image) const;

  
  void ClampHotspot();

  
  int type_;

  
  gfx::Point hotspot_;

  
  
  gfx::Size custom_size_;  
  float custom_scale_;
  std::vector<char> custom_data_;

#if defined(OS_WIN)
  
  
  HCURSOR external_cursor_;
#endif

#if defined(USE_AURA) && defined(USE_X11)
  
  ui::PlatformCursor platform_cursor_;
  float device_scale_factor_;
  gfx::Display::Rotation rotation_;
#elif defined(OS_WIN)
  
  HCURSOR custom_cursor_;
#elif defined(TOOLKIT_GTK)
  
  GdkCursor* unref_;
#endif
};

#endif  
