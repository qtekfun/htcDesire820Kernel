// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_ENGINE_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_ENGINE_H_

#include <map>
#include <string>
#include <vector>
#include "chrome/browser/chromeos/input_method/input_method_engine_interface.h"
#include "ui/base/ime/chromeos/ibus_bridge.h"
#include "url/gurl.h"

namespace ui {
class KeyEvent;
}  

namespace chromeos {

class IBusText;

namespace input_method {
class CandidateWindow;
struct InputMethodProperty;
struct KeyEventHandle;
}  

class InputMethodEngine : public InputMethodEngineInterface,
                              public IBusEngineHandlerInterface {
 public:
  InputMethodEngine();

  virtual ~InputMethodEngine();

  void Initialize(
      InputMethodEngineInterface::Observer* observer,
      const char* engine_name,
      const char* extension_id,
      const char* engine_id,
      const std::vector<std::string>& languages,
      const std::vector<std::string>& layouts,
      const GURL& options_page,
      const GURL& input_view);

  
  virtual void StartIme() OVERRIDE;
  virtual bool SetComposition(int context_id,
                              const char* text,
                              int selection_start,
                              int selection_end,
                              int cursor,
                              const std::vector<SegmentInfo>& segments,
                              std::string* error) OVERRIDE;
  virtual bool ClearComposition(int context_id, std::string* error) OVERRIDE;
  virtual bool CommitText(int context_id, const char* text,
                          std::string* error) OVERRIDE;
  virtual const CandidateWindowProperty&
    GetCandidateWindowProperty() const OVERRIDE;
  virtual void SetCandidateWindowProperty(
      const CandidateWindowProperty& property) OVERRIDE;
  virtual bool SetCandidateWindowVisible(bool visible,
                                         std::string* error) OVERRIDE;
  virtual void SetCandidateWindowAuxText(const char* text) OVERRIDE;
  virtual void SetCandidateWindowAuxTextVisible(bool visible) OVERRIDE;
  virtual bool SetCandidates(int context_id,
                             const std::vector<Candidate>& candidates,
                             std::string* error) OVERRIDE;
  virtual bool SetCursorPosition(int context_id, int candidate_id,
                                 std::string* error) OVERRIDE;
  virtual bool SetMenuItems(const std::vector<MenuItem>& items) OVERRIDE;
  virtual bool UpdateMenuItems(const std::vector<MenuItem>& items) OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual void KeyEventDone(input_method::KeyEventHandle* key_data,
                            bool handled) OVERRIDE;
  virtual bool DeleteSurroundingText(int context_id,
                                     int offset,
                                     size_t number_of_chars,
                                     std::string* error) OVERRIDE;

  
  virtual void FocusIn(
      const IBusEngineHandlerInterface::InputContext& input_context) OVERRIDE;
  virtual void FocusOut() OVERRIDE;
  virtual void Enable() OVERRIDE;
  virtual void Disable() OVERRIDE;
  virtual void PropertyActivate(const std::string& property_name) OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void ProcessKeyEvent(const ui::KeyEvent& key_event,
                               const KeyEventDoneCallback& callback) OVERRIDE;
  virtual void CandidateClicked(uint32 index) OVERRIDE;
  virtual void SetSurroundingText(const std::string& text, uint32 cursor_pos,
                                  uint32 anchor_pos) OVERRIDE;

 private:
  
  void MenuItemToProperty(const MenuItem& item,
                          input_method::InputMethodProperty* property);

  
  bool focused_;

  
  bool active_;

  
  int context_id_;

  
  int next_context_id_;

  
  std::string engine_id_;

  
  std::string ibus_id_;

  
  std::string aux_text_;
  bool aux_text_visible_;

  
  InputMethodEngineInterface::Observer* observer_;

  
  scoped_ptr<IBusText> preedit_text_;
  int preedit_cursor_;

  
  scoped_ptr<input_method::CandidateWindow> candidate_window_;

  
  CandidateWindowProperty candidate_window_property_;

  
  bool window_visible_;

  
  std::vector<int> candidate_ids_;

  
  std::map<int, int> candidate_indexes_;

  
  GURL input_view_url_;
};

}  

#endif  
