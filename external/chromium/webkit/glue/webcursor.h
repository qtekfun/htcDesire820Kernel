// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_WEBCURSOR_H_
#define WEBKIT_GLUE_WEBCURSOR_H_

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

#include <vector>

#if defined(OS_WIN)
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HICON__* HICON;
typedef HICON HCURSOR;
#elif defined(USE_X11)
typedef struct _GdkCursor GdkCursor;
#elif defined(OS_MACOSX)
#ifdef __OBJC__
@class NSCursor;
#else
class NSCursor;
#endif
typedef UInt32 ThemeCursor;
struct Cursor;
#endif

class Pickle;

namespace WebKit {
class WebImage;
struct WebCursorInfo;
}

class WebCursor {
 public:
  WebCursor();
  explicit WebCursor(const WebKit::WebCursorInfo& cursor_info);
  ~WebCursor();

  
  WebCursor(const WebCursor& other);
  const WebCursor& operator=(const WebCursor& other);

  
  void InitFromCursorInfo(const WebKit::WebCursorInfo& cursor_info);
  void GetCursorInfo(WebKit::WebCursorInfo* cursor_info) const;

  
  bool Deserialize(const Pickle* pickle, void** iter);
  bool Serialize(Pickle* pickle) const;

  
  
  bool IsCustom() const;

  
  
  
  bool IsEqual(const WebCursor& other) const;

  
  gfx::NativeCursor GetNativeCursor();

#if defined(OS_WIN)
  
  
  
  HCURSOR GetCursor(HINSTANCE module_handle);

  
  
  
  void InitFromExternalCursor(HCURSOR handle);

#elif defined(USE_X11)
  
  
  
  
  int GetCursorType() const;

  
  
  GdkCursor* GetCustomCursor();
#elif defined(OS_MACOSX)
  
  NSCursor* GetCursor() const;

  
  void InitFromThemeCursor(ThemeCursor cursor);

  
  void InitFromCursor(const Cursor* cursor);

  
  void InitFromNSCursor(NSCursor* cursor);
#endif

 private:
  
  void Copy(const WebCursor& other);

  
  void Clear();

  
  void InitPlatformData();

  
  bool SerializePlatformData(Pickle* pickle) const;
  bool DeserializePlatformData(const Pickle* pickle, void** iter);

  
  
  bool IsPlatformDataEqual(const WebCursor& other) const ;

  
  void CopyPlatformData(const WebCursor& other);

  
  void CleanupPlatformData();

  void SetCustomData(const WebKit::WebImage& image);
  void ImageFromCustomData(WebKit::WebImage* image) const;

  
  void ClampHotspot();

  
  int type_;

  gfx::Point hotspot_;

  
  
  gfx::Size custom_size_;
  std::vector<char> custom_data_;

#if defined(OS_WIN)
  
  
  HCURSOR external_cursor_;
  
  HCURSOR custom_cursor_;
#endif  

#if defined(USE_X11)
  
  GdkCursor* unref_;
#endif
};

#endif  
