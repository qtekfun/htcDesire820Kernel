// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LIBGTK2UI_CHROME_GTK_FRAME_H_
#define CHROME_BROWSER_UI_LIBGTK2UI_CHROME_GTK_FRAME_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _MetaFrames       MetaFrames;
typedef struct _MetaFramesClass  MetaFramesClass;

struct _MetaFrames {
  GtkWindow window;
};

struct _MetaFramesClass {
  GtkWindowClass parent_class;
};


typedef struct _ChromeGtkFrame       ChromeGtkFrame;
typedef struct _ChromeGtkFrameClass  ChromeGtkFrameClass;

struct _ChromeGtkFrame {
  MetaFrames frames;
};

struct _ChromeGtkFrameClass {
  MetaFramesClass frames_class;
};

GtkWidget* chrome_gtk_frame_new();

G_END_DECLS

#endif  
