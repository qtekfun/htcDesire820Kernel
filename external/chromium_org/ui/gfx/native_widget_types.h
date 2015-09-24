// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_NATIVE_WIDGET_TYPES_H_
#define UI_GFX_NATIVE_WIDGET_TYPES_H_

#include "build/build_config.h"

#if defined(OS_ANDROID)
#include <jni.h>
#endif

#include "base/basictypes.h"
#include "base/logging.h"
#include "ui/gfx/gfx_export.h"


#if defined(USE_AURA)
class SkRegion;
namespace aura {
class Window;
}
namespace ui {
class Cursor;
class Event;
}
#endif  

#if defined(OS_WIN)
#include <windows.h>  
typedef struct HFONT__* HFONT;
struct IAccessible;
#elif defined(OS_IOS)
struct CGContext;
#ifdef __OBJC__
@class UIEvent;
@class UIFont;
@class UIImage;
@class UIView;
@class UIWindow;
@class UITextField;
#else
class UIEvent;
class UIFont;
class UIImage;
class UIView;
class UIWindow;
class UITextField;
#endif  
#elif defined(OS_MACOSX)
struct CGContext;
#ifdef __OBJC__
@class NSCursor;
@class NSEvent;
@class NSFont;
@class NSImage;
@class NSView;
@class NSWindow;
@class NSTextField;
#else
class NSCursor;
class NSEvent;
class NSFont;
class NSImage;
struct NSView;
class NSWindow;
class NSTextField;
#endif  
#elif defined(OS_POSIX)
typedef struct _PangoFontDescription PangoFontDescription;
typedef struct _cairo cairo_t;
#endif

#if defined(TOOLKIT_GTK)
typedef struct _GdkCursor GdkCursor;
typedef union _GdkEvent GdkEvent;
typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GdkRegion GdkRegion;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;
#elif defined(OS_ANDROID)
struct ANativeWindow;
namespace ui {
class WindowAndroid;
class ViewAndroid;
}
#endif
class SkBitmap;

namespace gfx {

#if defined(USE_AURA)
typedef ui::Cursor NativeCursor;
typedef aura::Window* NativeView;
typedef aura::Window* NativeWindow;
typedef SkRegion* NativeRegion;
typedef ui::Event* NativeEvent;
#elif defined(OS_WIN)
typedef HCURSOR NativeCursor;
typedef HWND NativeView;
typedef HWND NativeWindow;
typedef HRGN NativeRegion;
typedef MSG NativeEvent;
#elif defined(OS_IOS)
typedef void* NativeCursor;
typedef UIView* NativeView;
typedef UIWindow* NativeWindow;
typedef UIEvent* NativeEvent;
#elif defined(OS_MACOSX)
typedef NSCursor* NativeCursor;
typedef NSView* NativeView;
typedef NSWindow* NativeWindow;
typedef NSEvent* NativeEvent;
#elif defined(TOOLKIT_GTK)
typedef GdkCursor* NativeCursor;
typedef GtkWidget* NativeView;
typedef GtkWindow* NativeWindow;
typedef GdkRegion* NativeRegion;
typedef GdkEvent* NativeEvent;
#elif defined(OS_ANDROID)
typedef void* NativeCursor;
typedef ui::ViewAndroid* NativeView;
typedef ui::WindowAndroid* NativeWindow;
typedef void* NativeRegion;
typedef jobject NativeEvent;
#endif

#if defined(OS_WIN)
typedef HFONT NativeFont;
typedef HWND NativeEditView;
typedef HDC NativeDrawingContext;
typedef IAccessible* NativeViewAccessible;
#elif defined(OS_IOS)
typedef UIFont* NativeFont;
typedef UITextField* NativeEditView;
typedef CGContext* NativeDrawingContext;
#elif defined(OS_MACOSX)
typedef NSFont* NativeFont;
typedef NSTextField* NativeEditView;
typedef CGContext* NativeDrawingContext;
typedef void* NativeViewAccessible;
#elif defined(TOOLKIT_GTK)
typedef PangoFontDescription* NativeFont;
typedef GtkWidget* NativeEditView;
typedef cairo_t* NativeDrawingContext;
typedef void* NativeViewAccessible;
#elif defined(USE_CAIRO)
typedef PangoFontDescription* NativeFont;
typedef void* NativeEditView;
typedef cairo_t* NativeDrawingContext;
typedef void* NativeViewAccessible;
#else
typedef void* NativeFont;
typedef void* NativeEditView;
typedef void* NativeDrawingContext;
typedef void* NativeViewAccessible;
#endif

#if defined(USE_AURA)
const int kNullCursor = 0;
#else
const gfx::NativeCursor kNullCursor = static_cast<gfx::NativeCursor>(NULL);
#endif

#if defined(OS_IOS)
typedef UIImage NativeImageType;
#elif defined(OS_MACOSX)
typedef NSImage NativeImageType;
#elif defined(TOOLKIT_GTK)
typedef GdkPixbuf NativeImageType;
#else
typedef SkBitmap NativeImageType;
#endif
typedef NativeImageType* NativeImage;

typedef intptr_t NativeViewId;

#if defined(OS_WIN) && !defined(USE_AURA)
static inline NativeView NativeViewFromId(NativeViewId id) {
  return reinterpret_cast<NativeView>(id);
}
#define NativeViewFromIdInBrowser(x) NativeViewFromId(x)
#elif defined(OS_POSIX) || defined(USE_AURA)

static inline NativeView NativeViewFromIdInBrowser(NativeViewId id) {
  return reinterpret_cast<NativeView>(id);
}
#endif  

#if defined(OS_WIN)
  typedef HWND PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = NULL;
#elif defined(USE_X11)
  typedef unsigned long PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = 0;
#elif defined(USE_AURA) && defined(OS_MACOSX)
  
  
  typedef NSView* PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = 0;
#elif defined(OS_ANDROID)
  typedef uint64 PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = 0;
#elif defined(USE_OZONE)
  typedef intptr_t PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = 0;
#else
  
  
  
  
  
  
  
  
  
  
  typedef uint64 PluginWindowHandle;
  const PluginWindowHandle kNullPluginWindow = 0;
#endif

enum SurfaceType {
  EMPTY,
  NATIVE_DIRECT,
  NATIVE_TRANSPORT,
  TEXTURE_TRANSPORT
};

struct GLSurfaceHandle {
  GLSurfaceHandle()
      : handle(kNullPluginWindow),
        transport_type(EMPTY),
        parent_gpu_process_id(0),
        parent_client_id(0) {
  }
  GLSurfaceHandle(PluginWindowHandle handle_, SurfaceType transport_)
      : handle(handle_),
        transport_type(transport_),
        parent_gpu_process_id(0),
        parent_client_id(0) {
    DCHECK(!is_null() || handle == kNullPluginWindow);
    DCHECK(transport_type != TEXTURE_TRANSPORT ||
           handle == kNullPluginWindow);
  }
  bool is_null() const { return transport_type == EMPTY; }
  bool is_transport() const {
    return transport_type == NATIVE_TRANSPORT ||
           transport_type == TEXTURE_TRANSPORT;
  }
  PluginWindowHandle handle;
  SurfaceType transport_type;
  int parent_gpu_process_id;
  uint32 parent_client_id;
};

#if defined(OS_WIN)
typedef HWND AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = NULL;
#elif defined(USE_X11)
typedef unsigned long AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = 0;
#elif defined(OS_IOS)
typedef UIView* AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = 0;
#elif defined(OS_MACOSX)
typedef NSView* AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = 0;
#elif defined(OS_ANDROID)
typedef ANativeWindow* AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = 0;
#elif defined(USE_OZONE)
typedef intptr_t AcceleratedWidget;
const AcceleratedWidget kNullAcceleratedWidget = 0;
#else
#error unknown platform
#endif

}  

#endif  
