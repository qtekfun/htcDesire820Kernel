// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_GTK_KEY_BINDINGS_HANDLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_GTK_KEY_BINDINGS_HANDLER_H_

#include <gtk/gtk.h>

#include <string>

#include "content/common/edit_command.h"
#include "content/common/content_export.h"
#include "ui/base/gtk/owned_widget_gtk.h"

namespace content {
struct NativeWebKeyboardEvent;

class CONTENT_EXPORT GtkKeyBindingsHandler {
 public:
  explicit GtkKeyBindingsHandler(GtkWidget* parent_widget);
  ~GtkKeyBindingsHandler();

  
  
  
  bool Match(const NativeWebKeyboardEvent& wke,
             EditCommands* edit_commands);

 private:
  
  struct Handler {
    GtkTextView parent_object;
    GtkKeyBindingsHandler *owner;
  };

  
  struct HandlerClass {
    GtkTextViewClass parent_class;
  };

  
  GtkWidget* CreateNewHandler();

  
  void EditCommandMatched(const std::string& name, const std::string& value);

  
  static void HandlerInit(Handler *self);

  
  static void HandlerClassInit(HandlerClass *klass);

  
  static GType HandlerGetType();

  
  static GtkKeyBindingsHandler* GetHandlerOwner(GtkTextView* text_view);

  
  static void BackSpace(GtkTextView* text_view);

  
  static void CopyClipboard(GtkTextView* text_view);

  
  static void CutClipboard(GtkTextView* text_view);

  
  static void DeleteFromCursor(GtkTextView* text_view, GtkDeleteType type,
                               gint count);

  
  static void InsertAtCursor(GtkTextView* text_view, const gchar* str);

  
  static void MoveCursor(GtkTextView* text_view, GtkMovementStep step,
                         gint count, gboolean extend_selection);

  
  static void MoveViewport(GtkTextView* text_view, GtkScrollStep step,
                           gint count);

  
  static void PasteClipboard(GtkTextView* text_view);

  
  static void SelectAll(GtkTextView* text_view, gboolean select);

  
  static void SetAnchor(GtkTextView* text_view);

  
  static void ToggleCursorVisible(GtkTextView* text_view);

  
  static void ToggleOverwrite(GtkTextView* text_view);

  
  static gboolean ShowHelp(GtkWidget* widget, GtkWidgetHelpType arg1);

  
  static void MoveFocus(GtkWidget* widget, GtkDirectionType arg1);

  ui::OwnedWidgetGtk handler_;

  
  EditCommands edit_commands_;
};

}  

#endif  
