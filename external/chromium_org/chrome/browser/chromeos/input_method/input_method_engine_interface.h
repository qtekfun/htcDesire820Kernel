// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_ENGINE_INTERFACE_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_ENGINE_INTERFACE_H_

#include <string>
#include <vector>

class GURL;

namespace chromeos {

namespace input_method {
struct KeyEventHandle;
}  

class InputMethodEngineInterface {
 public:
  struct KeyboardEvent {
    KeyboardEvent();
    virtual ~KeyboardEvent();

    std::string type;
    std::string key;
    std::string code;
    bool alt_key;
    bool ctrl_key;
    bool shift_key;
    bool caps_lock;
  };

  enum {
    MENU_ITEM_MODIFIED_LABEL        = 0x0001,
    MENU_ITEM_MODIFIED_STYLE        = 0x0002,
    MENU_ITEM_MODIFIED_VISIBLE      = 0x0004,
    MENU_ITEM_MODIFIED_ENABLED      = 0x0008,
    MENU_ITEM_MODIFIED_CHECKED      = 0x0010,
    MENU_ITEM_MODIFIED_ICON         = 0x0020,
  };

  enum MenuItemStyle {
    MENU_ITEM_STYLE_NONE,
    MENU_ITEM_STYLE_CHECK,
    MENU_ITEM_STYLE_RADIO,
    MENU_ITEM_STYLE_SEPARATOR,
  };

  enum MouseButtonEvent {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,
  };

  enum SegmentStyle {
    SEGMENT_STYLE_UNDERLINE,
    SEGMENT_STYLE_DOUBLE_UNDERLINE,
  };

  enum CandidateWindowPosition {
    WINDOW_POS_CURSOR,
    WINDOW_POS_COMPOSITTION,
  };

  struct MenuItem {
    MenuItem();
    virtual ~MenuItem();

    std::string id;
    std::string label;
    MenuItemStyle style;
    bool visible;
    bool enabled;
    bool checked;

    unsigned int modified;
    std::vector<MenuItem> children;
  };

  struct InputContext {
    int id;
    std::string type;
  };

  struct UsageEntry {
    std::string title;
    std::string body;
  };

  struct Candidate {
    Candidate();
    virtual ~Candidate();

    std::string value;
    int id;
    std::string label;
    std::string annotation;
    UsageEntry usage;
    std::vector<Candidate> candidates;
  };

  struct CandidateWindowProperty {
    CandidateWindowProperty();
    virtual ~CandidateWindowProperty();
    int page_size;
    bool is_cursor_visible;
    bool is_vertical;
    bool show_window_at_composition;
  };

  struct SegmentInfo {
    int start;
    int end;
    SegmentStyle style;
  };

  class Observer {
   public:
    virtual ~Observer();

    
    virtual void OnActivate(const std::string& engine_id) = 0;

    
    virtual void OnDeactivated(const std::string& engine_id) = 0;

    
    virtual void OnFocus(const InputContext& context) = 0;

    
    virtual void OnBlur(int context_id) = 0;

    
    virtual void OnInputContextUpdate(const InputContext& context) = 0;

    
    virtual void OnKeyEvent(const std::string& engine_id,
                            const KeyboardEvent& event,
                            input_method::KeyEventHandle* key_data) = 0;

    
    virtual void OnCandidateClicked(const std::string& engine_id,
                                    int candidate_id,
                                    MouseButtonEvent button) = 0;

    
    virtual void OnMenuItemActivated(const std::string& engine_id,
                                     const std::string& menu_id) = 0;

    
    virtual void OnSurroundingTextChanged(const std::string& engine_id,
                                          const std::string& text,
                                          int cursor_pos,
                                          int anchor_pos) = 0;

    
    virtual void OnReset(const std::string& engine_id) = 0;
  };

  virtual ~InputMethodEngineInterface() {}

  
  
  
  virtual void StartIme() = 0;

  
  virtual bool SetComposition(int context_id,
                              const char* text,
                              int selection_start,
                              int selection_end,
                              int cursor,
                              const std::vector<SegmentInfo>& segments,
                              std::string* error) = 0;

  
  virtual bool ClearComposition(int context_id, std::string* error) = 0;

  
  
  virtual bool CommitText(int context_id, const char* text,
                          std::string* error) = 0;

  
  
  
  virtual const CandidateWindowProperty&
    GetCandidateWindowProperty() const = 0;

  
  
  virtual void SetCandidateWindowProperty(
      const CandidateWindowProperty& property) = 0;

  
  virtual bool SetCandidateWindowVisible(bool visible, std::string* error) = 0;

  
  virtual void SetCandidateWindowAuxText(const char* text) = 0;

  
  virtual void SetCandidateWindowAuxTextVisible(bool visible) = 0;

  
  virtual bool SetCandidates(int context_id,
                             const std::vector<Candidate>& candidates,
                             std::string* error) = 0;

  
  virtual bool SetCursorPosition(int context_id, int candidate_id,
                                 std::string* error) = 0;

  
  
  virtual bool SetMenuItems(const std::vector<MenuItem>& items) = 0;

  
  virtual bool UpdateMenuItems(const std::vector<MenuItem>& items) = 0;

  
  virtual bool IsActive() const = 0;

  
  virtual void KeyEventDone(input_method::KeyEventHandle* key_data,
                            bool handled) = 0;

  
  
  
  
  
  
  
  virtual bool DeleteSurroundingText(int context_id,
                                     int offset,
                                     size_t number_of_chars,
                                     std::string* error) = 0;
};

}  

#endif  
