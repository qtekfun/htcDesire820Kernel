// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_WIN_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_WIN_H_
#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <tom.h>  

#include "base/memory/scoped_ptr.h"
#include "base/win/scoped_comptr.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_edit_view.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/browser/ui/views/autocomplete/autocomplete_popup_contents_view.h"
#include "content/common/page_transition_types.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/gfx/font.h"
#include "views/controls/menu/menu_2.h"
#include "webkit/glue/window_open_disposition.h"

class Profile;
class TabContents;
namespace views {
class View;
}

class AutocompleteEditController;
class AutocompleteEditModel;
class AutocompleteEditView;
class AutocompletePopupView;
class LocationBarView;

class AutocompleteEditViewWin
    : public CWindowImpl<AutocompleteEditViewWin,
                         CRichEditCtrl,
                         CWinTraits<WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |
                                    ES_NOHIDESEL> >,
      public CRichEditCommands<AutocompleteEditViewWin>,
      public ui::SimpleMenuModel::Delegate,
      public AutocompleteEditView {
 public:
  struct State {
    State(const CHARRANGE& selection,
          const CHARRANGE& saved_selection_for_focus_change)
        : selection(selection),
          saved_selection_for_focus_change(saved_selection_for_focus_change) {
    }

    const CHARRANGE selection;
    const CHARRANGE saved_selection_for_focus_change;
  };

  DECLARE_WND_CLASS(L"Chrome_AutocompleteEditView");

  AutocompleteEditViewWin(const gfx::Font& font,
                          AutocompleteEditController* controller,
                          ToolbarModel* toolbar_model,
                          LocationBarView* parent_view,
                          HWND hwnd,
                          Profile* profile,
                          CommandUpdater* command_updater,
                          bool popup_window_mode,
                          const views::View* location_bar);
  ~AutocompleteEditViewWin();

  views::View* parent_view() const;

  
  
  
  int WidthOfTextAfterCursor();

  
  gfx::Font GetFont();

  
  virtual AutocompleteEditModel* model() { return model_.get(); }
  virtual const AutocompleteEditModel* model() const { return model_.get(); }

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
  virtual int TextWidth() const;
  virtual string16 GetInstantSuggestion() const;
  virtual bool IsImeComposing() const;

  virtual views::View* AddToView(views::View* parent);
  virtual int OnPerformDrop(const views::DropTargetEvent& event);

  int GetPopupMaxYCoordinate();

  
  IAccessible* GetIAccessible();

  void SetDropHighlightPosition(int position);
  int drop_highlight_position() const { return drop_highlight_position_; }

  
  bool in_drag() const { return in_drag_; }

  
  void MoveSelectedText(int new_position);

  
  void InsertText(int position, const string16& text);

  
  
  
  void PasteAndGo(const string16& text);

  void set_force_hidden(bool force_hidden) { force_hidden_ = force_hidden; }

  
  
  bool SkipDefaultKeyEventProcessing(const views::KeyEvent& event);

  
  
  void HandleExternalMsg(UINT msg, UINT flags, const CPoint& screen_point);

  
  BEGIN_MSG_MAP(AutocompleteEdit)
    MSG_WM_CHAR(OnChar)
    MSG_WM_CONTEXTMENU(OnContextMenu)
    MSG_WM_COPY(OnCopy)
    MSG_WM_CUT(OnCut)
    MESSAGE_HANDLER_EX(WM_GETOBJECT, OnGetObject)
    MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeComposition)
    MSG_WM_KEYDOWN(OnKeyDown)
    MSG_WM_KEYUP(OnKeyUp)
    MSG_WM_KILLFOCUS(OnKillFocus)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
    MSG_WM_LBUTTONDOWN(OnLButtonDown)
    MSG_WM_LBUTTONUP(OnLButtonUp)
    MSG_WM_MBUTTONDBLCLK(OnMButtonDblClk)
    MSG_WM_MBUTTONDOWN(OnMButtonDown)
    MSG_WM_MBUTTONUP(OnMButtonUp)
    MSG_WM_MOUSEACTIVATE(OnMouseActivate)
    MSG_WM_MOUSEMOVE(OnMouseMove)
    MSG_WM_MOUSEWHEEL(OnMouseWheel)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_PASTE(OnPaste)
    MSG_WM_RBUTTONDBLCLK(OnRButtonDblClk)
    MSG_WM_RBUTTONDOWN(OnRButtonDown)
    MSG_WM_RBUTTONUP(OnRButtonUp)
    MSG_WM_SETFOCUS(OnSetFocus)
    MSG_WM_SETTEXT(OnSetText)
    MSG_WM_SYSCHAR(OnSysChar)  
    MSG_WM_SYSKEYDOWN(OnKeyDown)
    MSG_WM_SYSKEYUP(OnKeyUp)
    MSG_WM_WINDOWPOSCHANGING(OnWindowPosChanging)
    DEFAULT_REFLECTION_HANDLER()  
  END_MSG_MAP()

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual bool IsItemForCommandIdDynamic(int command_id) const;
  virtual string16 GetLabelForCommandId(int command_id) const;
  virtual void ExecuteCommand(int command_id);

  
  bool IsCaretAtEnd() const;

 private:
  enum MouseButton {
    kLeft  = 0,
    kRight = 1,
  };

  
  
  
  
  
  
  
  class ScopedFreeze {
   public:
    ScopedFreeze(AutocompleteEditViewWin* edit,
                 ITextDocument* text_object_model);
    ~ScopedFreeze();

   private:
    AutocompleteEditViewWin* const edit_;
    ITextDocument* const text_object_model_;

    DISALLOW_COPY_AND_ASSIGN(ScopedFreeze);
  };

  class EditDropTarget;
  friend class EditDropTarget;

  
  
  
  
  class ScopedSuspendUndo {
   public:
    explicit ScopedSuspendUndo(ITextDocument* text_object_model);
    ~ScopedSuspendUndo();

   private:
    ITextDocument* const text_object_model_;

    DISALLOW_COPY_AND_ASSIGN(ScopedSuspendUndo);
  };

  
  static int CALLBACK WordBreakProc(LPTSTR edit_text,
                                    int current_pos,
                                    int num_bytes,
                                    int action);

  
  static bool SchemeEnd(LPTSTR edit_text, int current_pos, int length);

  
  void OnChar(TCHAR ch, UINT repeat_count, UINT flags);
  void OnContextMenu(HWND window, const CPoint& point);
  void OnCopy();
  void OnCut();
  LRESULT OnGetObject(UINT uMsg, WPARAM wparam, LPARAM lparam);
  LRESULT OnImeComposition(UINT message, WPARAM wparam, LPARAM lparam);
  void OnKeyDown(TCHAR key, UINT repeat_count, UINT flags);
  void OnKeyUp(TCHAR key, UINT repeat_count, UINT flags);
  void OnKillFocus(HWND focus_wnd);
  void OnLButtonDblClk(UINT keys, const CPoint& point);
  void OnLButtonDown(UINT keys, const CPoint& point);
  void OnLButtonUp(UINT keys, const CPoint& point);
  void OnMButtonDblClk(UINT keys, const CPoint& point);
  void OnMButtonDown(UINT keys, const CPoint& point);
  void OnMButtonUp(UINT keys, const CPoint& point);
  LRESULT OnMouseActivate(HWND window, UINT hit_test, UINT mouse_message);
  void OnMouseMove(UINT keys, const CPoint& point);
  BOOL OnMouseWheel(UINT flags, short delta, CPoint point);
  void OnPaint(HDC bogus_hdc);
  void OnPaste();
  void OnRButtonDblClk(UINT keys, const CPoint& point);
  void OnRButtonDown(UINT keys, const CPoint& point);
  void OnRButtonUp(UINT keys, const CPoint& point);
  void OnSetFocus(HWND focus_wnd);
  LRESULT OnSetText(const wchar_t* text);
  void OnSysChar(TCHAR ch, UINT repeat_count, UINT flags);
  void OnWindowPosChanging(WINDOWPOS* window_pos);

  
  
  void HandleKeystroke(UINT message, TCHAR key, UINT repeat_count, UINT flags);

  
  
  
  bool OnKeyDownOnlyWritable(TCHAR key, UINT repeat_count, UINT flags);
  bool OnKeyDownAllModes(TCHAR key, UINT repeat_count, UINT flags);

  
  
  
  
  void GetSelection(CHARRANGE& sel) const;

  
  string16 GetSelectedText() const;

  
  
  
  void SetSelection(LONG start, LONG end);

  
  void SetSelectionRange(const CHARRANGE& sel) {
    SetSelection(sel.cpMin, sel.cpMax);
  }

  
  void PlaceCaretAt(size_t pos);

  
  
  bool IsSelectAllForRange(const CHARRANGE& sel) const;

  
  
  
  
  
  
  
  
  
  LONG ClipXCoordToVisibleText(LONG x, bool is_triple_click) const;

  
  
  
  void EmphasizeURLComponents();

  
  
  
  void EraseTopOfSelection(CDC* dc,
                           const CRect& client_rect,
                           const CRect& paint_clip_rect);

  
  void DrawSlashForInsecureScheme(HDC hdc,
                                  const CRect& client_rect,
                                  const CRect& paint_clip_rect);

  
  void DrawDropHighlight(HDC hdc,
                         const CRect& client_rect,
                         const CRect& paint_clip_rect);

  
  void TextChanged();

  
  
  
  string16 GetClipboardText() const;

  
  bool CanPasteAndGo(const string16& text) const;

  
  
  
  ITextDocument* GetTextObjectModel() const;

  
  void StartDragIfNecessary(const CPoint& point);

  
  
  
  void OnPossibleDrag(const CPoint& point);

  
  
  
  void RepaintDropHighlight(int position);

  
  void BuildContextMenu();

  void SelectAllIfNecessary(MouseButton button, const CPoint& point);
  void TrackMousePosition(MouseButton button, const CPoint& point);

  
  int GetHorizontalMargin() const;

  
  int WidthNeededToDisplay(const string16& text) const;

  
  
  
  bool OnAfterPossibleChangeInternal(bool force_text_changed);

  
  int OnPerformDropImpl(const views::DropTargetEvent& event, bool in_drag);

  scoped_ptr<AutocompleteEditModel> model_;

  scoped_ptr<AutocompletePopupView> popup_view_;

  AutocompleteEditController* controller_;

  
  
  LocationBarView* parent_view_;

  ToolbarModel* toolbar_model_;

  
  
  CommandUpdater* command_updater_;

  
  
  bool popup_window_mode_;

  
  
  
  
  bool force_hidden_;

  
  
  
  
  
  
  
  
  
  scoped_ptr<ScopedFreeze> gaining_focus_;

  
  
  
  
  
  
  
  bool tracking_click_[2];
  CPoint click_point_[2];

  
  
  bool tracking_double_click_;
  CPoint double_click_point_;
  DWORD double_click_time_;

  
  
  bool can_discard_mousemove_;

  
  
  bool ignore_ime_messages_;

  
  string16 text_before_change_;
  CHARRANGE sel_before_change_;

  
  
  CHARRANGE original_selection_;

  
  
  CHARRANGE saved_selection_for_focus_change_;

  
  bool delete_at_end_pressed_;

  
  scoped_ptr<ui::SimpleMenuModel> context_menu_contents_;
  scoped_ptr<views::Menu2> context_menu_;

  
  
  gfx::Font font_;

  
  
  
  
  int font_x_height_;
  int font_y_adjustment_;

  
  
  bool possible_drag_;

  
  bool in_drag_;

  
  
  
  bool initiated_drag_;

  
  int drop_highlight_position_;

  
  COLORREF background_color_;
  ToolbarModel::SecurityLevel security_level_;

  
  mutable ITextDocument* text_object_model_;

  
  
  url_parse::Component insecure_scheme_component_;

  
  mutable base::win::ScopedComPtr<IAccessible> autocomplete_accessibility_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteEditViewWin);
};

#endif  
