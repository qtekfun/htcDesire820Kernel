// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_X11_UTIL_H_
#define UI_BASE_X_X11_UTIL_H_


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/memory/ref_counted_memory.h"
#include "ui/base/ui_export.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/point.h"
#include "ui/gfx/x/x11_types.h"

typedef unsigned long Atom;
typedef unsigned long XSharedMemoryId;  
typedef unsigned long Cursor;
typedef struct _XcursorImage XcursorImage;
typedef union _XEvent XEvent;

#if defined(TOOLKIT_GTK)
typedef struct _GdkDrawable GdkWindow;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;
#endif

namespace gfx {
class Canvas;
class Point;
class Rect;
}
class SkBitmap;

namespace ui {



UI_EXPORT bool XDisplayExists();

UI_EXPORT bool IsXInput2Available();

enum SharedMemorySupport {
  SHARED_MEMORY_NONE,
  SHARED_MEMORY_PUTIMAGE,
  SHARED_MEMORY_PIXMAP
};
UI_EXPORT SharedMemorySupport QuerySharedMemorySupport(XDisplay* dpy);

UI_EXPORT bool QueryRenderSupport(XDisplay* dpy);

int GetDefaultScreen(XDisplay* display);

UI_EXPORT ::Cursor GetXCursor(int cursor_shape);

UI_EXPORT void ResetXCursorCache();

#if defined(USE_AURA)
UI_EXPORT ::Cursor CreateReffedCustomXCursor(XcursorImage* image);

UI_EXPORT void RefCustomXCursor(::Cursor cursor);

UI_EXPORT void UnrefCustomXCursor(::Cursor cursor);

UI_EXPORT XcursorImage* SkBitmapToXcursorImage(const SkBitmap* bitmap,
                                               const gfx::Point& hotspot);

UI_EXPORT int CoalescePendingMotionEvents(const XEvent* xev,
                                          XEvent* last_event);
#endif

UI_EXPORT void HideHostCursor();

UI_EXPORT ::Cursor CreateInvisibleCursor();


UI_EXPORT XID GetX11RootWindow();

bool GetCurrentDesktop(int* desktop);

#if defined(TOOLKIT_GTK)
UI_EXPORT XID GetX11WindowFromGtkWidget(GtkWidget* widget);
XID GetX11WindowFromGdkWindow(GdkWindow* window);

UI_EXPORT GtkWindow* GetGtkWindowFromX11Window(XID xid);

UI_EXPORT void* GetVisualFromGtkWidget(GtkWidget* widget);
#endif  

enum HideTitlebarWhenMaximized {
  SHOW_TITLEBAR_WHEN_MAXIMIZED = 0,
  HIDE_TITLEBAR_WHEN_MAXIMIZED = 1,
};
UI_EXPORT void SetHideTitlebarWhenMaximizedProperty(
    XID window,
    HideTitlebarWhenMaximized property);

UI_EXPORT void ClearX11DefaultRootWindow();

UI_EXPORT bool IsWindowVisible(XID window);

UI_EXPORT bool GetWindowRect(XID window, gfx::Rect* rect);

UI_EXPORT bool WindowContainsPoint(XID window, gfx::Point screen_loc);

UI_EXPORT bool PropertyExists(XID window, const std::string& property_name);

UI_EXPORT bool GetRawBytesOfProperty(
    XID window,
    Atom property,
    scoped_refptr<base::RefCountedMemory>* out_data,
    size_t* out_data_bytes,
    size_t* out_data_items,
    Atom* out_type);

UI_EXPORT bool GetIntProperty(XID window, const std::string& property_name,
                              int* value);
UI_EXPORT bool GetXIDProperty(XID window, const std::string& property_name,
                              XID* value);
UI_EXPORT bool GetIntArrayProperty(XID window, const std::string& property_name,
                                   std::vector<int>* value);
UI_EXPORT bool GetAtomArrayProperty(XID window,
                                    const std::string& property_name,
                                    std::vector<Atom>* value);
UI_EXPORT bool GetStringProperty(
    XID window, const std::string& property_name, std::string* value);

UI_EXPORT bool SetIntProperty(XID window,
                              const std::string& name,
                              const std::string& type,
                              int value);
UI_EXPORT bool SetIntArrayProperty(XID window,
                                   const std::string& name,
                                   const std::string& type,
                                   const std::vector<int>& value);
UI_EXPORT bool SetAtomArrayProperty(XID window,
                                    const std::string& name,
                                    const std::string& type,
                                    const std::vector<Atom>& value);

Atom GetAtom(const char* atom_name);

UI_EXPORT void SetWindowClassHint(XDisplay* display,
                                  XID window,
                                  const std::string& res_name,
                                  const std::string& res_class);

UI_EXPORT void SetWindowRole(XDisplay* display,
                             XID window,
                             const std::string& role);

UI_EXPORT XID GetParentWindow(XID window);

XID GetHighestAncestorWindow(XID window, XID root);

static const int kAllDesktops = -1;
bool GetWindowDesktop(XID window, int* desktop);

UI_EXPORT std::string GetX11ErrorString(XDisplay* display, int err);

class EnumerateWindowsDelegate {
 public:
  
  
  virtual bool ShouldStopIterating(XID xid) = 0;

 protected:
  virtual ~EnumerateWindowsDelegate() {}
};

UI_EXPORT bool EnumerateAllWindows(EnumerateWindowsDelegate* delegate,
                                   int max_depth);

UI_EXPORT void EnumerateTopLevelWindows(ui::EnumerateWindowsDelegate* delegate);

UI_EXPORT bool GetXWindowStack(XID window, std::vector<XID>* windows);

void RestackWindow(XID window, XID sibling, bool above);

UI_EXPORT XSharedMemoryId AttachSharedMemory(XDisplay* display,
                                             int shared_memory_support);
UI_EXPORT void DetachSharedMemory(XDisplay* display, XSharedMemoryId shmseg);

UI_EXPORT bool CopyAreaToCanvas(XID drawable,
                                gfx::Rect source_bounds,
                                gfx::Point dest_offset,
                                gfx::Canvas* canvas);

UI_EXPORT XID CreatePictureFromSkiaPixmap(XDisplay* display, XID pixmap);

void FreePicture(XDisplay* display, XID picture);
void FreePixmap(XDisplay* display, XID pixmap);

enum WindowManagerName {
  WM_UNKNOWN,
  WM_BLACKBOX,
  WM_CHROME_OS,
  WM_COMPIZ,
  WM_ENLIGHTENMENT,
  WM_ICE_WM,
  WM_KWIN,
  WM_METACITY,
  WM_MUFFIN,
  WM_MUTTER,
  WM_OPENBOX,
  WM_XFWM4,
};
UI_EXPORT WindowManagerName GuessWindowManager();

UI_EXPORT bool ChangeWindowDesktop(XID window, XID destination);

UI_EXPORT void SetDefaultX11ErrorHandlers();

UI_EXPORT bool IsX11WindowFullScreen(XID window);

UI_EXPORT bool IsXDisplaySizeBlackListed(unsigned long mm_width,
                                         unsigned long mm_height);

class UI_EXPORT XRefcountedMemory : public base::RefCountedMemory {
 public:
  XRefcountedMemory(unsigned char* x11_data, size_t length)
      : x11_data_(length ? x11_data : NULL),
        length_(length) {
  }

  
  virtual const unsigned char* front() const OVERRIDE;
  virtual size_t size() const OVERRIDE;

 private:
  virtual ~XRefcountedMemory();

  unsigned char* x11_data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(XRefcountedMemory);
};

class UI_EXPORT XScopedString {
 public:
  explicit XScopedString(char* str) : string_(str) {}
  ~XScopedString();

  const char* string() const { return string_; }

 private:
  char* string_;

  DISALLOW_COPY_AND_ASSIGN(XScopedString);
};

class UI_EXPORT XScopedImage {
 public:
  explicit XScopedImage(XImage* image) : image_(image) {}
  ~XScopedImage();

  XImage* get() const {
    return image_;
  }

  XImage* operator->() const {
    return image_;
  }

  void reset(XImage* image);

 private:
  XImage* image_;

  DISALLOW_COPY_AND_ASSIGN(XScopedImage);
};

class UI_EXPORT XScopedCursor {
 public:
  
  XScopedCursor(::Cursor cursor, XDisplay* display);
  ~XScopedCursor();

  ::Cursor get() const;
  void reset(::Cursor cursor);

 private:
  ::Cursor cursor_;
  XDisplay* display_;

  DISALLOW_COPY_AND_ASSIGN(XScopedCursor);
};

}  

#endif  
