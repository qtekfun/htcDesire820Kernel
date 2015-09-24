// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MESSAGE_BOX_VIEW_H_
#define UI_VIEWS_CONTROLS_MESSAGE_BOX_VIEW_H_

#include <vector>

#include "base/strings/string16.h"
#include "ui/views/view.h"

namespace gfx {
class ImageSkia;
}

namespace views {

class Checkbox;
class ImageView;
class Label;
class Link;
class LinkListener;
class Textfield;

class VIEWS_EXPORT MessageBoxView : public View {
 public:
  enum Options {
    NO_OPTIONS = 0,
    
    
    
    
    
    
    
    DETECT_DIRECTIONALITY = 1 << 0,
    HAS_PROMPT_FIELD = 1 << 1,
  };

  struct VIEWS_EXPORT InitParams {
    explicit InitParams(const string16& message);
    ~InitParams();

    uint16 options;
    string16 message;
    string16 default_prompt;
    int message_width;
    int inter_row_vertical_spacing;
  };

  explicit MessageBoxView(const InitParams& params);

  virtual ~MessageBoxView();

  
  views::Textfield* text_box() { return prompt_field_; }

  
  string16 GetInputText();

  
  
  bool IsCheckBoxSelected();

  
  
  void SetIcon(const gfx::ImageSkia& icon);

  
  
  
  void SetCheckBoxLabel(const string16& label);

  
  void SetCheckBoxSelected(bool selected);

  
  
  void SetLink(const string16& text, LinkListener* listener);

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

 private:
  
  
  void Init(const InitParams& params);

  
  
  void ResetLayoutManager();

  
  std::vector<Label*> message_labels_;

  
  Textfield* prompt_field_;

  
  ImageView* icon_;

  
  Checkbox* checkbox_;

  
  Link* link_;

  
  int message_width_;

  
  int inter_row_vertical_spacing_;

  DISALLOW_COPY_AND_ASSIGN(MessageBoxView);
};

}  

#endif  
