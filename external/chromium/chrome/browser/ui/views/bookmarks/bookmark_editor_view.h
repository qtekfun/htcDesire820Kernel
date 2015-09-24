// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_EDITOR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_EDITOR_VIEW_H_
#pragma once

#include "base/string16.h"
#include "chrome/browser/bookmarks/bookmark_editor.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/models/tree_node_model.h"
#include "views/controls/button/button.h"
#include "views/controls/textfield/textfield.h"
#include "views/controls/textfield/textfield_controller.h"
#include "views/controls/tree/tree_view.h"
#include "views/window/dialog_delegate.h"

namespace views {
class Label;
class Menu2;
class NativeButton;
class Window;
}

class BookmarkEditorViewTest;
class GURL;
class Menu;
class Profile;


class BookmarkEditorView : public BookmarkEditor,
                           public views::View,
                           public views::ButtonListener,
                           public views::TreeViewController,
                           public views::DialogDelegate,
                           public views::TextfieldController,
                           public views::ContextMenuController,
                           public ui::SimpleMenuModel::Delegate,
                           public BookmarkModelObserver {
 public:
  
  typedef ui::TreeNodeWithValue<int64> EditorNode;

  
  
  class EditorTreeModel : public ui::TreeNodeModel<EditorNode> {
   public:
    explicit EditorTreeModel(EditorNode* root)
        : ui::TreeNodeModel<EditorNode>(root) {}

    virtual void SetTitle(ui::TreeModelNode* node,
                          const string16& title) {
      if (!title.empty())
        TreeNodeModel::SetTitle(node, title);
    }

   private:
    DISALLOW_COPY_AND_ASSIGN(EditorTreeModel);
  };

  BookmarkEditorView(Profile* profile,
                     const BookmarkNode* parent,
                     const EditDetails& details,
                     BookmarkEditor::Configuration configuration);

  virtual ~BookmarkEditorView();

  
  virtual bool IsDialogButtonEnabled(
      MessageBoxFlags::DialogButton button) const;
  virtual bool IsModal() const;
  virtual bool CanResize() const;
  virtual std::wstring GetWindowTitle() const;
  virtual bool Accept();
  virtual bool AreAcceleratorsEnabled(MessageBoxFlags::DialogButton button);
  virtual views::View* GetContentsView();

  
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

  
  virtual void OnTreeViewSelectionChanged(views::TreeView* tree_view);
  virtual bool CanEdit(views::TreeView* tree_view, ui::TreeModelNode* node);

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const std::wstring& new_contents);
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& key_event) {
    return false;
  }

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

  
  
  void Show(HWND parent_hwnd);

  
  void Close();

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& p,
                                      bool is_mouse_gesture);

 private:
  friend class BookmarkEditorViewTest;

  
  
  void Init();

  
  
  virtual void Loaded(BookmarkModel* model) { }
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index);
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index);
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int index,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) {}
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node) {}

  
  void Reset();

  
  
  
  void ExpandAndSelect();

  
  
  EditorNode* CreateRootNode();

  
  
  void CreateNodes(const BookmarkNode* bb_node, EditorNode* b_node);

  
  EditorNode* FindNodeWithID(BookmarkEditorView::EditorNode* node, int64 id);

  
  void ApplyEdits();

  
  
  void ApplyEdits(EditorNode* parent);

  
  
  
  
  
  
  
  
  
  void ApplyNameChangesAndCreateNewFolders(
      const BookmarkNode* bb_node,
      BookmarkEditorView::EditorNode* b_node,
      BookmarkEditorView::EditorNode* parent_b_node,
      const BookmarkNode** parent_bb_node);

  
  GURL GetInputURL() const;

  
  std::wstring GetInputTitle() const;

  
  
  void UserInputChanged();

  
  
  
  void NewFolder();

  
  
  
  EditorNode* AddNewFolder(EditorNode* parent);

  
  Profile* profile_;

  
  scoped_ptr<EditorTreeModel> tree_model_;

  
  views::TreeView* tree_view_;

  
  scoped_ptr<views::NativeButton> new_folder_button_;

  
  views::Label* url_label_;

  
  views::Textfield url_tf_;

  
  views::Label* title_label_;

  
  views::Textfield title_tf_;

  
  
  const BookmarkNode* parent_;

  const EditDetails details_;

  
  scoped_ptr<ui::SimpleMenuModel> context_menu_contents_;
  scoped_ptr<views::Menu2> context_menu_;

  
  BookmarkModel* bb_model_;

  
  
  bool running_menu_for_root_;

  
  bool show_tree_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkEditorView);
};

#endif  
