// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_G_OBJECT_DESTRUCTOR_FILO_H_
#define UI_BASE_GTK_G_OBJECT_DESTRUCTOR_FILO_H_

#include <glib.h>
#include <list>
#include <map>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

template <typename T> struct DefaultSingletonTraits;

typedef struct _GObject GObject;

namespace ui {

class UI_EXPORT GObjectDestructorFILO {
 public:
  typedef void (*DestructorHook)(void* context, GObject* where_the_object_was);

  static GObjectDestructorFILO* GetInstance();
  void Connect(GObject* object, DestructorHook callback, void* context);
  void Disconnect(GObject* object, DestructorHook callback, void* context);

 private:
  struct Hook {
    Hook(GObject* o, DestructorHook cb, void* ctx)
        : object(o), callback(cb), context(ctx) {
    }
    bool equal(GObject* o, DestructorHook cb, void* ctx) const {
      return object == o && callback == cb && context == ctx;
    }
    GObject* object;
    DestructorHook callback;
    void* context;
  };
  typedef std::list<Hook> HandlerList;
  typedef std::map<GObject*, HandlerList> HandlerMap;

  GObjectDestructorFILO();
  ~GObjectDestructorFILO();
  friend struct DefaultSingletonTraits<GObjectDestructorFILO>;

  void WeakNotify(GObject* where_the_object_was);
  static void WeakNotifyThunk(gpointer data, GObject* where_the_object_was) {
    reinterpret_cast<GObjectDestructorFILO*>(data)->WeakNotify(
        where_the_object_was);
  }

  HandlerMap handler_map_;

  DISALLOW_COPY_AND_ASSIGN(GObjectDestructorFILO);
};

}  

#endif  
