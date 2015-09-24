// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GTK_NATIVE_VIEW_ID_MANAGER_H_
#define UI_GFX_GTK_NATIVE_VIEW_ID_MANAGER_H_

#include <map>

#include "base/synchronization/lock.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

template <typename T> struct DefaultSingletonTraits;

typedef unsigned long XID;
struct _GtkPreserveWindow;

class GFX_EXPORT GtkNativeViewManager {
 public:
  
  static GtkNativeViewManager* GetInstance();

  
  
  
  
  
  
  
  gfx::NativeViewId GetIdForWidget(gfx::NativeView widget);

  
  
  
  
  
  
  
  
  bool GetXIDForId(XID* xid, gfx::NativeViewId id);

  
  
  
  bool GetNativeViewForId(gfx::NativeView* xid, gfx::NativeViewId id);

  
  
  
  
  
  
  
  
  
  bool GetPermanentXIDForId(XID* xid, gfx::NativeViewId id);

  
  
  
  bool AddRefPermanentXID(XID xid);

  
  
  
  
  
  
  void ReleasePermanentXID(XID xid);

  
  void OnRealize(gfx::NativeView widget);
  void OnUnrealize(gfx::NativeView widget);
  void OnDestroy(gfx::NativeView widget);

 private:
  
  GtkNativeViewManager();
  ~GtkNativeViewManager();
  friend struct DefaultSingletonTraits<GtkNativeViewManager>;

  struct NativeViewInfo {
    NativeViewInfo() : widget(NULL), x_window_id(0) {
    }
    gfx::NativeView widget;
    XID x_window_id;
  };

  gfx::NativeViewId GetWidgetId(gfx::NativeView id);

  
  base::Lock lock_;

  
  
  std::map<gfx::NativeView, gfx::NativeViewId> native_view_to_id_;
  std::map<gfx::NativeViewId, NativeViewInfo> id_to_info_;

  struct PermanentXIDInfo {
    PermanentXIDInfo() : widget(NULL), ref_count(0) {
    }
    _GtkPreserveWindow* widget;
    int ref_count;
  };

  
  
  
  
  
  
  
  
  
  
  
  std::map<XID, PermanentXIDInfo> perm_xid_to_info_;

  DISALLOW_COPY_AND_ASSIGN(GtkNativeViewManager);
};

#endif  
