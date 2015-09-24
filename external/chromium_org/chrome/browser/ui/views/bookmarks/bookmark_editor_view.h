// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_EDITOR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_EDITOR_VIEW_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "chrome/browser/bookmarks/bookmark_expanded_state_tracker.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/ui/bookmarks/bookmark_editor.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/models/tree_node_model.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/controls/tree/tree_view_controller.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class Label;
class LabelButton;
class MenuRunner;
class TreeView;
}

class BookmarkEditorViewTest;
class GURL;
class Menu;
class Profile;


class BookmarkEditorView : public BookmarkEditor,
                           public views::ButtonListener,
                           public views::TreeViewController,
                           public views::DialogDelegateView,
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
                          const base::string16& title) OVERRIDE;

   private:
    DISALLOW_COPY_AND_ASSIGN(EditorTreeModel);
  };

  BookmarkEditorView(Profile* profile,
                     const BookmarkNode* parent,
                     const EditDetails& details,
                     BookmarkEditor::Configuration configuration);

  virtual ~BookmarkEditorView();

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual views::View* CreateExtraView() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual bool CanResize() const  OVERRIDE;
  virtual base::string16 GetWindowTitle() const  OVERRIDE;
  virtual bool Accept() OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnTreeViewSelectionChanged(views::TreeView* tree_view) OVERRIDE;
  virtual bool CanEdit(views::TreeView* tree_view,
                       ui::TreeModelNode* node) OVERRIDE;

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  
  void Show(gfx::NativeWindow parent);

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

 private:
  friend class BookmarkEditorViewTest;

  
  
  void Init();

  
  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE {}
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE;
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE {}
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE {}

  
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

  
  
  void UserInputChanged();

  
  
  
  void NewFolder();

  
  
  
  EditorNode* AddNewFolder(EditorNode* parent);

  
  
  void UpdateExpandedNodes(EditorNode* editor_node,
                           BookmarkExpandedStateTracker::Nodes* expanded_nodes);

  ui::SimpleMenuModel* GetMenuModel();

  
  Profile* profile_;

  
  scoped_ptr<EditorTreeModel> tree_model_;

  
  views::TreeView* tree_view_;

  
  scoped_ptr<views::LabelButton> new_folder_button_;

  
  views::Label* url_label_;

  
  views::Textfield* url_tf_;

  
  views::Label* title_label_;

  
  views::Textfield* title_tf_;

  
  
  const BookmarkNode* parent_;

  const EditDetails details_;

  
  scoped_ptr<ui::SimpleMenuModel> context_menu_model_;
  scoped_ptr<views::MenuRunner> context_menu_runner_;

  
  BookmarkModel* bb_model_;

  
  
  bool running_menu_for_root_;

  
  bool show_tree_;

  
  std::vector<int64> deletes_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkEditorView);
};

#endif  
