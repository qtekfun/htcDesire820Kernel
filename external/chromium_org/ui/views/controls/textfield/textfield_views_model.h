// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_VIEWS_MODEL_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_VIEWS_MODEL_H_

#include <list>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/ime/composition_text.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/render_text.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class Range;
class RenderText;
}  

namespace views {

namespace internal {
class Edit;

enum MergeType {
  
  
  DO_NOT_MERGE,
  
  MERGEABLE,
  
  
  
  MERGE_WITH_PREVIOUS,
};

}  

class VIEWS_EXPORT TextfieldViewsModel {
 public:
  
  
  class VIEWS_EXPORT Delegate {
   public:
    
    virtual void OnCompositionTextConfirmedOrCleared() = 0;

   protected:
    virtual ~Delegate();
  };

  explicit TextfieldViewsModel(Delegate* delegate);
  virtual ~TextfieldViewsModel();

  

  const string16& GetText() const;
  
  
  
  
  
  
  bool SetText(const string16& text);

  gfx::RenderText* render_text() { return render_text_.get(); }

  
  
  void InsertText(const string16& text) {
    InsertTextInternal(text, false);
  }

  
  void InsertChar(char16 c) {
    InsertTextInternal(string16(&c, 1), true);
  }

  
  
  void ReplaceText(const string16& text) {
    ReplaceTextInternal(text, false);
  }

  
  void ReplaceChar(char16 c) {
    ReplaceTextInternal(string16(&c, 1), true);
  }

  
  
  void Append(const string16& text);

  
  
  
  
  bool Delete();

  
  
  
  
  bool Backspace();

  

  
  size_t GetCursorPosition() const;

  
  
  void MoveCursor(gfx::BreakType break_type,
                  gfx::VisualCursorDirection direction,
                  bool select);

  
  
  
  
  bool MoveCursorTo(const gfx::SelectionModel& selection);

  
  bool MoveCursorTo(const gfx::Point& point, bool select);

  

  
  string16 GetSelectedText() const;

  
  
  
  void SelectRange(const gfx::Range& range);

  
  
  void SelectSelectionModel(const gfx::SelectionModel& sel);

  
  
  
  
  void SelectAll(bool reversed);

  
  
  
  
  void SelectWord();

  
  
  void ClearSelection();

  
  bool CanUndo();

  
  bool CanRedo();

  
  bool Undo();

  
  bool Redo();

  
  
  bool Cut();

  
  
  bool Copy();

  
  
  bool Paste();

  
  
  bool HasSelection() const;

  
  
  void DeleteSelection();

  
  
  void DeleteSelectionAndInsertTextAt(
      const string16& text, size_t position);

  
  string16 GetTextFromRange(const gfx::Range& range) const;

  
  void GetTextRange(gfx::Range* range) const;

  
  
  
  
  
  
  void SetCompositionText(const ui::CompositionText& composition);

  
  void ConfirmCompositionText();

  
  void CancelCompositionText();

  
  void GetCompositionTextRange(gfx::Range* range) const;

  
  bool HasCompositionText() const;

 private:
  friend class NativeTextfieldViews;
  friend class NativeTextfieldViewsTest;
  friend class TextfieldViewsModelTest;
  friend class UndoRedo_BasicTest;
  friend class UndoRedo_CutCopyPasteTest;
  friend class UndoRedo_ReplaceTest;
  friend class internal::Edit;

  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_BasicTest);
  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_CutCopyPasteTest);
  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_ReplaceTest);

  
  
  void InsertTextInternal(const string16& text, bool mergeable);

  
  
  
  void ReplaceTextInternal(const string16& text, bool mergeable);

  
  void ClearEditHistory();

  
  void ClearRedoHistory();

  
  void ExecuteAndRecordDelete(gfx::Range range, bool mergeable);
  void ExecuteAndRecordReplaceSelection(internal::MergeType merge_type,
                                        const string16& text);
  void ExecuteAndRecordReplace(internal::MergeType merge_type,
                               size_t old_cursor_pos,
                               size_t new_cursor_pos,
                               const string16& text,
                               size_t new_text_start);
  void ExecuteAndRecordInsert(const string16& text, bool mergeable);

  
  
  bool AddOrMergeEditHistory(internal::Edit* edit);

  
  
  
  
  
  void ModifyText(size_t delete_from,
                  size_t delete_to,
                  const string16& new_text,
                  size_t new_text_insert_at,
                  size_t new_cursor_pos);

  void ClearComposition();

  
  
  Delegate* delegate_;

  
  scoped_ptr<gfx::RenderText> render_text_;

  typedef std::list<internal::Edit*> EditHistory;
  EditHistory edit_history_;

  
  
  
  
  
  
  
  
  
  
  
  EditHistory::iterator current_edit_;

  DISALLOW_COPY_AND_ASSIGN(TextfieldViewsModel);
};

}  

#endif  
