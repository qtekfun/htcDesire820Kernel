// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_GTK_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <algorithm>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string_util.h"
#include "chrome/browser/autocomplete/autocomplete_edit_view.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/gtk_signal_registrar.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteEditController;
class AutocompleteEditModel;
class AutocompletePopupView;
class Profile;
class TabContents;

namespace gfx {
class Font;
}

namespace ui {
class MultiAnimation;
}

namespace views {
class View;
}

#if !defined(TOOLKIT_VIEWS)
class GtkThemeService;
#endif

class AutocompleteEditViewGtk : public AutocompleteEditView,
                                public NotificationObserver,
                                public ui::AnimationDelegate {
 public:
  
  
  
  struct CharRange {
    CharRange() : cp_min(0), cp_max(0) { }
    CharRange(int n, int x) : cp_min(n), cp_max(x) { }

    
    int selection_min() const { return std::min(cp_min, cp_max); }
    int selection_max() const { return std::max(cp_min, cp_max); }

    
    int cp_min;  
    int cp_max;  
  };

  AutocompleteEditViewGtk(AutocompleteEditController* controller,
                          ToolbarModel* toolbar_model,
                          Profile* profile,
                          CommandUpdater* command_updater,
                          bool popup_window_mode,
#if defined(TOOLKIT_VIEWS)
                          views::View* location_bar
#else
                          GtkWidget* location_bar
#endif
                          );
  virtual ~AutocompleteEditViewGtk();

  
  void Init();
  
  
  
  int WidthOfTextAfterCursor();

  
  virtual AutocompleteEditModel* model();
  virtual const AutocompleteEditModel* model() const;

  virtual void SaveStateToTab(TabContents* tab);

  virtual void Update(const TabContents* tab_for_state_restoring);

  virtual void OpenURL(const GURL& url,
                       WindowOpenDisposition disposition,
                       PageTransition::Type transition,
                       const GURL& alternate_nav_url,
                       size_t selected_line,
                       const string16& keyword);

  virtual string16 GetText() const;

  virtual bool IsEditingOrEmpty() const;
  virtual int GetIcon() const;

  virtual void SetUserText(const string16& text);
  virtual void SetUserText(const string16& text,
                           const string16& display_text,
                           bool update_popup);

  virtual void SetWindowTextAndCaretPos(const string16& text,
                                        size_t caret_pos);

  virtual void SetForcedQuery();

  virtual bool IsSelectAll();
  virtual bool DeleteAtEndPressed();
  virtual void GetSelectionBounds(string16::size_type* start,
                                  string16::size_type* end);
  virtual void SelectAll(bool reversed);
  virtual void RevertAll();

  virtual void UpdatePopup();
  virtual void ClosePopup();

  virtual void SetFocus();

  virtual void OnTemporaryTextMaybeChanged(const string16& display_text,
                                           bool save_original_selection);
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const string16& display_text, size_t user_text_length);
  virtual void OnRevertTemporaryText();
  virtual void OnBeforePossibleChange();
  virtual bool OnAfterPossibleChange();
  virtual gfx::NativeView GetNativeView() const;
  virtual CommandUpdater* GetCommandUpdater();
  virtual void SetInstantSuggestion(const string16& suggestion,
                                    bool animate_to_complete);
  virtual string16 GetInstantSuggestion() const;
  virtual int TextWidth() const;
  virtual bool IsImeComposing() const;

#if defined(TOOLKIT_VIEWS)
  virtual views::View* AddToView(views::View* parent);
  virtual int OnPerformDrop(const views::DropTargetEvent& event);

  
  
  
  
  static AutocompleteEditView* Create(AutocompleteEditController* controller,
                                      ToolbarModel* toolbar_model,
                                      Profile* profile,
                                      CommandUpdater* command_updater,
                                      bool popup_window_mode,
                                      views::View* location_bar);
#endif

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

  
  void SetBaseColor();
  
  
  void UpdateInstantViewColors();

  
  
  GtkWidget* text_view() {
    return text_view_;
  }

 private:
  CHROMEG_CALLBACK_0(AutocompleteEditViewGtk, void, HandleBeginUserAction,
                     GtkTextBuffer*);
  CHROMEG_CALLBACK_0(AutocompleteEditViewGtk, void, HandleEndUserAction,
                     GtkTextBuffer*);
  CHROMEG_CALLBACK_2(AutocompleteEditViewGtk, void, HandleMarkSet,
                     GtkTextBuffer*, GtkTextIter*, GtkTextMark*);
  
  CHROMEG_CALLBACK_2(AutocompleteEditViewGtk, void, HandleMarkSetAfter,
                     GtkTextBuffer*, GtkTextIter*, GtkTextMark*);
  CHROMEG_CALLBACK_3(AutocompleteEditViewGtk, void, HandleInsertText,
                     GtkTextBuffer*, GtkTextIter*, const gchar*, gint);
  CHROMEG_CALLBACK_0(AutocompleteEditViewGtk, void,
                     HandleKeymapDirectionChanged, GdkKeymap*);
  CHROMEG_CALLBACK_2(AutocompleteEditViewGtk, void, HandleDeleteRange,
                     GtkTextBuffer*, GtkTextIter*, GtkTextIter*);
  
  
  CHROMEG_CALLBACK_2(AutocompleteEditViewGtk, void, HandleMarkSetAlways,
                     GtkTextBuffer*, GtkTextIter*, GtkTextMark*);

  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleKeyPress,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleKeyRelease,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleViewButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean,
                       HandleViewButtonRelease, GdkEventButton*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleViewFocusIn,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleViewFocusOut,
                       GdkEventFocus*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void, HandleViewMoveFocus,
                       GtkDirectionType);
  CHROMEGTK_CALLBACK_3(AutocompleteEditViewGtk, void, HandleViewMoveCursor,
                       GtkMovementStep, gint, gboolean);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void, HandleViewSizeRequest,
                       GtkRequisition*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void, HandlePopulatePopup,
                       GtkMenu*);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleEditSearchEngines);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandlePasteAndGo);
  CHROMEGTK_CALLBACK_6(AutocompleteEditViewGtk, void, HandleDragDataReceived,
                       GdkDragContext*, gint, gint, GtkSelectionData*,
                       guint, guint);
  CHROMEGTK_CALLBACK_4(AutocompleteEditViewGtk, void, HandleDragDataGet,
                       GdkDragContext*, GtkSelectionData*, guint, guint);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleBackSpace);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleCopyClipboard);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleCutClipboard);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandlePasteClipboard);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, gboolean, HandleExposeEvent,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void,
                       HandleWidgetDirectionChanged, GtkTextDirection);
  CHROMEGTK_CALLBACK_2(AutocompleteEditViewGtk, void,
                       HandleDeleteFromCursor, GtkDeleteType, gint);
  
  
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void, HandleHierarchyChanged,
                       GtkWidget*);
#if GTK_CHECK_VERSION(2, 20, 0)
  CHROMEGTK_CALLBACK_1(AutocompleteEditViewGtk, void, HandlePreeditChanged,
                       const gchar*);
#endif
  
  
  
  
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleUndoRedo);
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void, HandleUndoRedoAfter);

  CHROMEG_CALLBACK_1(AutocompleteEditViewGtk, void, HandleWindowSetFocus,
                     GtkWindow*, GtkWidget*);

  
  CHROMEGTK_CALLBACK_0(AutocompleteEditViewGtk, void,
                       HandlePopupMenuDeactivate);

  
  static void ClipboardGetSelectionThunk(GtkClipboard* clipboard,
                                         GtkSelectionData* selection_data,
                                         guint info,
                                         gpointer object);
  void ClipboardGetSelection(GtkClipboard* clipboard,
                             GtkSelectionData* selection_data,
                             guint info);

  void HandleCopyOrCutClipboard(bool copy);

  
  bool OnPerformDropImpl(const string16& text);

  
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

  
  int GetTextLength() const;

  
  void PlaceCaretAt(int pos);

  
  bool IsCaretAtEnd() const;

  
  void EmphasizeURLComponents();

  
  void TextChanged();

  
  
  void SavePrimarySelection(const std::string& selected_text);

  
  void SetTextAndSelectedRange(const string16& text,
                               const CharRange& range);

  
  void SetSelectedRange(const CharRange& range);

  
  
  
  void AdjustTextJustification();

  
  
  PangoDirection GetContentDirection();

  
  std::string GetSelectedText() const;

  
  void UpdatePrimarySelectionIfValidURL();

  
  
  void GetTextBufferBounds(GtkTextIter* start, GtkTextIter* end) const;

  
  
  void ValidateTextBufferIter(GtkTextIter* iter) const;

  
  
  void AdjustVerticalAlignmentOfInstantView();

  
  void StopAnimation();

  
  
  OwnedWidgetGtk alignment_;

  
  
  
  
  GtkWidget* text_view_;

  GtkTextTagTable* tag_table_;
  GtkTextBuffer* text_buffer_;
  GtkTextTag* faded_text_tag_;
  GtkTextTag* secure_scheme_tag_;
  GtkTextTag* security_error_scheme_tag_;
  GtkTextTag* normal_text_tag_;

  
  GtkTextTag* instant_anchor_tag_;

  
  
  GtkWidget* instant_view_;
  
  
  scoped_ptr<ui::MultiAnimation> instant_animation_;

  
  
  
  GtkTextMark* instant_mark_;

  scoped_ptr<AutocompleteEditModel> model_;
  scoped_ptr<AutocompletePopupView> popup_view_;
  AutocompleteEditController* controller_;
  ToolbarModel* toolbar_model_;

  
  
  CommandUpdater* command_updater_;

  
  
  bool popup_window_mode_;

  ToolbarModel::SecurityLevel security_level_;

  
  
  CharRange saved_temporary_selection_;

  
  string16 text_before_change_;
  CharRange sel_before_change_;

  
  
  
  
  
  
  
  std::string selected_text_;

  
  std::string primary_selection_text_;

  
  gulong mark_set_handler_id_;
  gulong mark_set_handler_id2_;

#if defined(OS_CHROMEOS)
  
  
  

  
  
  
  bool button_1_pressed_;

  
  
  
  bool text_selected_during_click_;

  
  
  
  bool text_view_focused_before_button_press_;
#endif

#if defined(TOOLKIT_VIEWS)
  views::View* location_bar_view_;
#else
  
  GtkThemeService* theme_service_;

  NotificationRegistrar registrar_;
#endif

  
  
  
  
  
  bool enter_was_pressed_;

  
  
  
  
  bool tab_was_pressed_;

  
  
  
  bool paste_clipboard_requested_;

  
  
  
  bool enter_was_inserted_;

  
  
  
  bool text_changed_;

  
  
  
  CharRange strikethrough_;

  
  
  bool selection_suggested_;

  
  bool delete_was_pressed_;

  
  bool delete_at_end_pressed_;

  
  bool handling_key_press_;

  
  
  
  
  
  
  bool content_maybe_changed_by_key_press_;

  
  
  
  bool update_popup_without_focus_;

#if GTK_CHECK_VERSION(2, 20, 0)
  
  string16 preedit_;

  
  
  size_t preedit_size_before_change_;
#endif

  
  
  GtkWidget* going_to_focus_;

  ui::GtkSignalRegistrar signals_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteEditViewGtk);
};

#endif  
