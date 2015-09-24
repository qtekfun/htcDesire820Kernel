// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TREE_TREE_VIEW_VIEWS_H_
#define UI_VIEWS_CONTROLS_TREE_TREE_VIEW_VIEWS_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/models/tree_node_model.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/prefix_delegate.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/view.h"

namespace views {

class Textfield;
class TreeViewController;
class PrefixSelector;

class VIEWS_EXPORT TreeView : public ui::TreeModelObserver,
                              public TextfieldController,
                              public FocusChangeListener,
                              public PrefixDelegate {
 public:
  
  static const char kViewClassName[];

  TreeView();
  virtual ~TreeView();

  
  View* CreateParentIfNecessary();

  
  void SetModel(ui::TreeModel* model);
  ui::TreeModel* model() const { return model_; }

  
  
  
  
  
  void set_auto_expand_children(bool auto_expand_children) {
    auto_expand_children_ = auto_expand_children;
  }

  
  
  void SetEditable(bool editable);

  
  
  void StartEditing(ui::TreeModelNode* node);

  
  void CancelEdit();

  
  void CommitEdit();

  
  
  ui::TreeModelNode* GetEditingNode();

  
  void SetSelectedNode(ui::TreeModelNode* model_node);

  
  ui::TreeModelNode* GetSelectedNode();

  
  
  void Collapse(ui::TreeModelNode* model_node);

  
  void Expand(ui::TreeModelNode* node);

  
  
  void ExpandAll(ui::TreeModelNode* node);

  
  bool IsExpanded(ui::TreeModelNode* model_node);

  
  
  
  void SetRootShown(bool root_visible);

  
  
  void SetController(TreeViewController* controller) {
    controller_ = controller;
  }

  
  ui::TreeModelNode* GetNodeForRow(int row);

  
  int GetRowForNode(ui::TreeModelNode* node);

  views::Textfield* editor() { return editor_; }

  
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               ui::MenuSourceType source_type) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual void TreeNodesAdded(ui::TreeModel* model,
                              ui::TreeModelNode* parent,
                              int start,
                              int count) OVERRIDE;
  virtual void TreeNodesRemoved(ui::TreeModel* model,
                                ui::TreeModelNode* parent,
                                int start,
                                int count) OVERRIDE;
  virtual void TreeNodeChanged(ui::TreeModel* model,
                               ui::TreeModelNode* model_node) OVERRIDE;

  
  virtual void ContentsChanged(Textfield* sender,
                               const string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before,
                                View* focused_now) OVERRIDE;

  
  virtual int GetRowCount() OVERRIDE;
  virtual int GetSelectedRow() OVERRIDE;
  virtual void SetSelectedRow(int row) OVERRIDE;
  virtual string16 GetTextForRow(int row) OVERRIDE;

 protected:
  
  virtual gfx::Point GetKeyboardContextMenuLocation() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

 private:
  friend class TreeViewTest;

  
  
  bool OnClickOrTap(const ui::LocatedEvent& event);

  
  
  class InternalNode : public ui::TreeNode<InternalNode> {
   public:
    InternalNode();
    virtual ~InternalNode();

    
    void Reset(ui::TreeModelNode* node);

    
    ui::TreeModelNode* model_node() { return model_node_; }

    
    void set_is_expanded(bool expanded) { is_expanded_ = expanded; }
    bool is_expanded() const { return is_expanded_; }

    
    void set_loaded_children(bool value) { loaded_children_ = value; }
    bool loaded_children() const { return loaded_children_; }

    
    void set_text_width(int width) { text_width_ = width; }
    int text_width() const { return text_width_; }

    
    int NumExpandedNodes() const;

    
    
    
    int GetMaxWidth(int indent, int depth);

   private:
    
    ui::TreeModelNode* model_node_;

    
    bool loaded_children_;

    bool is_expanded_;

    int text_width_;

    DISALLOW_COPY_AND_ASSIGN(InternalNode);
  };

  
  enum GetInternalNodeCreateType {
    
    CREATE_IF_NOT_LOADED,

    
    DONT_CREATE_IF_NOT_LOADED,
  };

  
  enum IncrementType {
    
    INCREMENT_NEXT,

    
    INCREMENT_PREVIOUS
  };

  
  
  int root_row() const { return root_shown_ ? 0 : -1; }

  
  int root_depth() const { return root_shown_ ? 0 : -1; }

  
  void LoadChildren(InternalNode* node);

  
  
  void ConfigureInternalNode(ui::TreeModelNode* model_node, InternalNode* node);

  
  void UpdateNodeTextWidth(InternalNode* node);

  
  void DrawnNodesChanged();

  
  void UpdatePreferredSize();

  
  void LayoutEditor();

  
  void SchedulePaintForNode(InternalNode* node);

  
  
  
  void PaintRows(gfx::Canvas* canvas,
                 int min_row,
                 int max_row,
                 InternalNode* node,
                 int depth,
                 int* row);

  
  void PaintRow(gfx::Canvas* canvas,
                InternalNode* node,
                int row,
                int depth);

  
  void PaintExpandControl(gfx::Canvas* canvas,
                          const gfx::Rect& node_bounds,
                          bool expanded);

  
  
  InternalNode* GetInternalNodeForModelNode(
      ui::TreeModelNode* model_node,
      GetInternalNodeCreateType create_type);

  
  gfx::Rect GetBoundsForNode(InternalNode* node);

  
  
  gfx::Rect GetBoundsForNodeImpl(InternalNode* node, int row, int depth);

  
  int GetRowForInternalNode(InternalNode* node, int* depth);

  
  InternalNode* GetNodeByRow(int row, int* depth);

  
  InternalNode* GetNodeByRowImpl(InternalNode* node,
                                 int target_row,
                                 int current_depth,
                                 int* current_row,
                                 int* node_depth);

  
  void IncrementSelection(IncrementType type);

  
  
  void CollapseOrSelectParent();

  
  
  void ExpandOrSelectChild();

  
  
  bool ExpandImpl(ui::TreeModelNode* model_node);

  
  ui::TreeModel* model_;

  
  gfx::ImageSkia closed_icon_;
  gfx::ImageSkia open_icon_;

  
  std::vector<gfx::ImageSkia> icons_;

  
  InternalNode root_;

  
  InternalNode* selected_node_;

  bool editing_;

  
  
  
  Textfield* editor_;

  
  gfx::Size empty_editor_size_;

  
  
  FocusManager* focus_manager_;

  
  bool auto_expand_children_;

  
  bool editable_;

  
  TreeViewController* controller_;

  
  bool root_shown_;

  
  bool has_custom_icons_;

  
  gfx::Size preferred_size_;

  
  gfx::Font font_;

  
  int row_height_;

  
  
  int text_offset_;

  scoped_ptr<PrefixSelector> selector_;

  DISALLOW_COPY_AND_ASSIGN(TreeView);
};

}  

#endif  
