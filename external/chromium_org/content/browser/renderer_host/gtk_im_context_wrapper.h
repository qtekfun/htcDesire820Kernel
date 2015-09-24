// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_GTK_IM_CONTEXT_WRAPPER_H_
#define CONTENT_BROWSER_RENDERER_HOST_GTK_IM_CONTEXT_WRAPPER_H_

#include <gdk/gdk.h>
#include <pango/pango-attributes.h>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"
#include "ui/base/ime/composition_text.h"
#include "ui/base/ime/text_input_type.h"

typedef struct _GtkIMContext GtkIMContext;
typedef struct _GtkWidget GtkWidget;

namespace gfx {
class Rect;
}

namespace content {
class RenderWidgetHostViewGtk;
struct NativeWebKeyboardEvent;

class GtkIMContextWrapper {
 public:
  explicit GtkIMContextWrapper(RenderWidgetHostViewGtk* host_view);
  ~GtkIMContextWrapper();

  
  void ProcessKeyEvent(GdkEventKey* event);

  void UpdateInputMethodState(ui::TextInputType type,
                              bool can_compose_inline);
  void UpdateCaretBounds(const gfx::Rect& caret_bounds);
  void OnFocusIn();
  void OnFocusOut();
  bool is_focused() const { return is_focused_; }

  GtkWidget* BuildInputMethodsGtkMenu();

  void CancelComposition();

  void ConfirmComposition();

 private:
  
  
  bool NeedCommitByForwardingCharEvent() const;

  
  bool HasInputMethodResult() const;

  void ProcessFilteredKeyPressEvent(NativeWebKeyboardEvent* wke);
  void ProcessUnfilteredKeyPressEvent(NativeWebKeyboardEvent* wke);

  
  
  void ProcessInputMethodResult(const GdkEventKey* event, bool filtered);

  
  void HandleCommit(const base::string16& text);

  
  void HandlePreeditStart();

  
  void HandlePreeditChanged(const gchar* text,
                            PangoAttrList* attrs,
                            int cursor_position);

  
  void HandlePreeditEnd();

  
  gboolean HandleRetrieveSurrounding(GtkIMContext* context);

  
  
  void HandleHostViewRealize(GtkWidget* widget);

  
  
  void HandleHostViewUnrealize();

  
  
  void SendFakeCompositionKeyEvent(blink::WebInputEvent::Type type);

  
  static void HandleCommitThunk(GtkIMContext* context, gchar* text,
                                GtkIMContextWrapper* self);
  static void HandlePreeditStartThunk(GtkIMContext* context,
                                      GtkIMContextWrapper* self);
  static void HandlePreeditChangedThunk(GtkIMContext* context,
                                        GtkIMContextWrapper* self);
  static void HandlePreeditEndThunk(GtkIMContext* context,
                                    GtkIMContextWrapper* self);
  static gboolean HandleRetrieveSurroundingThunk(GtkIMContext* context,
                                                 GtkIMContextWrapper* self);

  
  static void HandleHostViewRealizeThunk(GtkWidget* widget,
                                         GtkIMContextWrapper* self);
  static void HandleHostViewUnrealizeThunk(GtkWidget* widget,
                                           GtkIMContextWrapper* self);

  
  RenderWidgetHostViewGtk* host_view_;

  
  
  
  
  
  
  
  
  
  
  
  
  GtkIMContext* context_;

  
  
  GtkIMContext* context_simple_;

  
  bool is_focused_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool is_composing_text_;

  
  bool is_enabled_;

  
  
  
  
  bool is_in_key_event_handler_;

  
  ui::CompositionText composition_;

  
  bool is_composition_changed_;

  
  
  base::string16 commit_text_;

  
  
  
  bool suppress_next_commit_;

  
  
  int last_key_code_;
  bool last_key_was_up_;
  bool last_key_filtered_no_result_;

  DISALLOW_COPY_AND_ASSIGN(GtkIMContextWrapper);
};

}  

#endif  
