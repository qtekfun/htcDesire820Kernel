// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_OMNIBOX_OMNIBOX_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_OMNIBOX_OMNIBOX_VIEW_GTK_H_

#include <gtk/gtk.h>

#include <algorithm>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_util.h"
#include "chrome/browser/ui/omnibox/omnibox_view.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/rect.h"

class Browser;
class OmniboxPopupView;
class Profile;

namespace gfx {
class Font;
}

class GtkThemeService;

class OmniboxViewGtk : public OmniboxView,
                       public content::NotificationObserver {
 public:
  
  
  
  struct CharRange {
    CharRange() : cp_min(0), cp_max(0) { }
    CharRange(int n, int x) : cp_min(n), cp_max(x) { }

    
    int selection_min() const { return std::min(cp_min, cp_max); }
    int selection_max() const { return std::max(cp_min, cp_max); }

    
    int cp_min;  
    int cp_max;  
  };

  
  
  
  
  OmniboxViewGtk(OmniboxEditController* controller,
                 Browser* browser,
                 Profile* profile,
                 CommandUpdater* command_updater,
                 bool popup_window_mode,
                 GtkWidget* location_bar);
  virtual ~OmniboxViewGtk();

  
  void Init();

  
  virtual void SaveStateToTab(content::WebContents* tab) OVERRIDE;
  virtual void OnTabChanged(const content::WebContents* web_contents) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual base::string16 GetText() const OVERRIDE;
  virtual void SetWindowTextAndCaretPos(const base::string16& text,
                                        size_t caret_pos,
                                        bool update_popup,
                                        bool notify_text_changed) OVERRIDE;
  virtual void SetForcedQuery() OVERRIDE;
  virtual bool IsSelectAll() const OVERRIDE;
  virtual bool DeleteAtEndPressed() OVERRIDE;
  virtual void GetSelectionBounds(
      base::string16::size_type* start,
      base::string16::size_type* end) const OVERRIDE;
  virtual void SelectAll(bool reversed) OVERRIDE;
  virtual void UpdatePopup() OVERRIDE;
  virtual void SetFocus() OVERRIDE;
  virtual void ApplyCaretVisibility() OVERRIDE;
  virtual void OnTemporaryTextMaybeChanged(
      const base::string16& display_text,
      bool save_original_selection,
      bool notify_text_changed) OVERRIDE;
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const base::string16& display_text, size_t user_text_length) OVERRIDE;
  virtual void OnInlineAutocompleteTextCleared() OVERRIDE;
  virtual void OnRevertTemporaryText() OVERRIDE;
  virtual void OnBeforePossibleChange() OVERRIDE;
  virtual bool OnAfterPossibleChange() OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeView GetRelativeWindowForPopup() const OVERRIDE;
  virtual void SetGrayTextAutocompletion(
      const base::string16& suggestion) OVERRIDE;
  virtual base::string16 GetGrayTextAutocompletion() const OVERRIDE;
  virtual int GetTextWidth() const OVERRIDE;
  virtual int GetWidth() const OVERRIDE;
  virtual bool IsImeComposing() const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void SetBaseColor();
  
  void UpdateGrayTextViewColors();

  
  
  GtkWidget* text_view() {
    return text_view_;
  }

 private:
  friend class OmniboxViewGtkTest;

  CHROMEG_CALLBACK_0(OmniboxViewGtk, void, HandleBeginUserAction,
                     GtkTextBuffer*);
  CHROMEG_CALLBACK_0(OmniboxViewGtk, void, HandleEndUserAction, GtkTextBuffer*);
  CHROMEG_CALLBACK_2(OmniboxViewGtk, void, HandleMarkSet, GtkTextBuffer*,
                     GtkTextIter*, GtkTextMark*);
  
  CHROMEG_CALLBACK_2(OmniboxViewGtk, void, HandleMarkSetAfter, GtkTextBuffer*,
                     GtkTextIter*, GtkTextMark*);
  CHROMEG_CALLBACK_3(OmniboxViewGtk, void, HandleInsertText, GtkTextBuffer*,
                     GtkTextIter*, const gchar*, gint);
  CHROMEG_CALLBACK_0(OmniboxViewGtk, void, HandleKeymapDirectionChanged,
                     GdkKeymap*);
  CHROMEG_CALLBACK_2(OmniboxViewGtk, void, HandleDeleteRange, GtkTextBuffer*,
                     GtkTextIter*, GtkTextIter*);
  
  
  CHROMEG_CALLBACK_2(OmniboxViewGtk, void, HandleMarkSetAlways, GtkTextBuffer*,
                     GtkTextIter*, GtkTextMark*);

  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleKeyPress, GdkEventKey*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleKeyRelease,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleViewButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleViewButtonRelease,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleViewFocusIn,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleViewFocusOut,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleViewMoveFocus,
                       GtkDirectionType);
  CHROMEGTK_CALLBACK_3(OmniboxViewGtk, void, HandleViewMoveCursor,
                       GtkMovementStep, gint, gboolean);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleViewSizeRequest,
                       GtkRequisition*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandlePopulatePopup, GtkMenu*);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandlePasteAndGo);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleShowURL);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleEditSearchEngines);
  CHROMEGTK_CALLBACK_6(OmniboxViewGtk, void, HandleDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);
  CHROMEGTK_CALLBACK_4(OmniboxViewGtk, void, HandleDragDataGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleBackSpace);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleCopyClipboard);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleCutClipboard);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandlePasteClipboard);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, gboolean, HandleExposeEvent,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleWidgetDirectionChanged,
                       GtkTextDirection);
  CHROMEGTK_CALLBACK_2(OmniboxViewGtk, void, HandleDeleteFromCursor,
                       GtkDeleteType, gint);
  
  
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandleHierarchyChanged,
                       GtkWidget*);
  CHROMEGTK_CALLBACK_1(OmniboxViewGtk, void, HandlePreEditChanged,
                       const gchar*);
  
  
  
  
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleUndoRedo);
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandleUndoRedoAfter);

  CHROMEG_CALLBACK_1(OmniboxViewGtk, void, HandleWindowSetFocus,
                     GtkWindow*, GtkWidget*);

  
  CHROMEGTK_CALLBACK_0(OmniboxViewGtk, void, HandlePopupMenuDeactivate);

  
  static void ClipboardGetSelectionThunk(GtkClipboard* clipboard,
                                         GtkSelectionData* selection_data,
                                         guint info,
                                         gpointer object);
  void ClipboardGetSelection(GtkClipboard* clipboard,
                             GtkSelectionData* selection_data,
                             guint info);

  void HandleCopyOrCutClipboard(bool copy);

  
  virtual int GetOmniboxTextLength() const OVERRIDE;
  virtual void EmphasizeURLComponents() OVERRIDE;

  
  bool OnPerformDropImpl(const base::string16& text);

  
  gfx::Font GetFont();

  
  
  
  void OwnPrimarySelection(const std::string& text);

  
  
  gfx::Rect WindowBoundsFromIters(GtkTextIter* iter1, GtkTextIter* iter2);

  
  
  
  void SelectAllInternal(bool reversed, bool update_primary_selection);

  
  
  
  void StartUpdatingHighlightedText();

  
  
  
  void FinishUpdatingHighlightedText();

  
  
  CharRange GetSelection() const;

  
  void ItersFromCharRange(const CharRange& range,
                          GtkTextIter* iter_min,
                          GtkTextIter* iter_max);

  
  bool IsCaretAtEnd() const;

  
  
  void SavePrimarySelection(const std::string& selected_text);

  
  void SetTextAndSelectedRange(const base::string16& text,
                               const CharRange& range);

  
  void SetSelectedRange(const CharRange& range);

  
  
  
  void AdjustTextJustification();

  
  
  PangoDirection GetContentDirection();

  
  std::string GetSelectedText() const;

  
  void UpdatePrimarySelectionIfValidURL();

  
  
  void GetTextBufferBounds(GtkTextIter* start, GtkTextIter* end) const;

  
  
  void ValidateTextBufferIter(GtkTextIter* iter) const;

  
  
  void AdjustVerticalAlignmentOfGrayTextView();

  
  Browser* browser_;

  
  
  ui::OwnedWidgetGtk alignment_;

  
  
  
  
  GtkWidget* text_view_;

  GtkTextTagTable* tag_table_;
  GtkTextBuffer* text_buffer_;
  GtkTextTag* faded_text_tag_;
  GtkTextTag* secure_scheme_tag_;
  GtkTextTag* security_error_scheme_tag_;
  GtkTextTag* normal_text_tag_;

  
  GtkTextTag* gray_text_anchor_tag_;

  
  
  GtkWidget* gray_text_view_;

  
  
  
  GtkTextMark* gray_text_mark_;

  scoped_ptr<OmniboxPopupView> popup_view_;

  
  
  bool popup_window_mode_;

  ToolbarModel::SecurityLevel security_level_;

  
  
  CharRange saved_temporary_selection_;

  
  base::string16 text_before_change_;
  CharRange sel_before_change_;

  
  
  
  
  
  
  
  std::string selected_text_;

  std::string dragged_text_;
  
  std::string primary_selection_text_;

  
  gulong mark_set_handler_id_;
  gulong mark_set_handler_id2_;

  
  
  
  bool button_1_pressed_;

  
  GtkThemeService* theme_service_;

  content::NotificationRegistrar registrar_;

  
  
  
  
  
  bool enter_was_pressed_;

  
  
  
  
  bool tab_was_pressed_;

  
  
  
  bool shift_was_pressed_;

  
  
  
  bool paste_clipboard_requested_;

  
  
  base::string16 sanitized_text_for_paste_and_go_;

  
  
  
  bool enter_was_inserted_;

  
  
  
  bool text_changed_;

  
  
  
  CharRange strikethrough_;

  
  
  bool selection_suggested_;

  
  bool delete_was_pressed_;

  
  bool delete_at_end_pressed_;

  
  bool handling_key_press_;

  
  
  
  
  
  
  bool content_maybe_changed_by_key_press_;

  
  
  
  bool update_popup_without_focus_;

  
  const bool supports_pre_edit_;

  
  base::string16 pre_edit_;

  
  
  size_t pre_edit_size_before_change_;

  
  
  GtkWidget* going_to_focus_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxViewGtk);
};

#endif  
