// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_CHROMEOS_IBUS_BRIDGE_H_
#define UI_BASE_IME_CHROMEOS_IBUS_BRIDGE_H_

#include <string>
#include "base/basictypes.h"
#include "base/callback.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_export.h"

namespace gfx {
class Rect;
}  

namespace ui {
class KeyEvent;
}  

namespace chromeos {
namespace input_method {
class CandidateWindow;
}  

class IBusText;

class UI_EXPORT IBusInputContextHandlerInterface {
 public:
  
  virtual void CommitText(const std::string& text) = 0;

  
  virtual void UpdatePreeditText(const IBusText& text,
                                 uint32 cursor_pos,
                                 bool visible) = 0;

  
  virtual void DeleteSurroundingText(int32 offset, uint32 length) = 0;
};


class UI_EXPORT IBusEngineHandlerInterface {
 public:
  typedef base::Callback<void (bool consumed)> KeyEventDoneCallback;

  
  
  
  struct InputContext {
    InputContext(ui::TextInputType type_, ui::TextInputMode mode_) :
      type(type_), mode(mode_) {}

    
    
    ui::TextInputType type;
    
    
    
    
    ui::TextInputMode mode;
  };

  virtual ~IBusEngineHandlerInterface() {}

  
  virtual void FocusIn(const InputContext& input_context) = 0;

  
  virtual void FocusOut() = 0;

  
  virtual void Enable() = 0;

  
  virtual void Disable() = 0;

  
  virtual void PropertyActivate(const std::string& property_name) = 0;

  
  virtual void Reset() = 0;

  
  
  virtual void ProcessKeyEvent(const ui::KeyEvent& key_event,
                               const KeyEventDoneCallback& callback) = 0;

  
  
  virtual void CandidateClicked(uint32 index) = 0;

  
  
  
  
  virtual void SetSurroundingText(const std::string& text, uint32 cursor_pos,
                                  uint32 anchor_pos) = 0;

 protected:
  IBusEngineHandlerInterface() {}
};

class UI_EXPORT IBusPanelCandidateWindowHandlerInterface {
 public:
  virtual ~IBusPanelCandidateWindowHandlerInterface() {}

  
  virtual void UpdateLookupTable(
      const input_method::CandidateWindow& candidate_window,
      bool visible) = 0;

  
  
  virtual void UpdateAuxiliaryText(const std::string& text, bool visible) = 0;

  
  
  virtual void UpdatePreeditText(const std::string& text, uint32 cursor_pos,
                                 bool visible) = 0;

  
  virtual void SetCursorBounds(const gfx::Rect& cursor_bounds,
                               const gfx::Rect& composition_head) = 0;

  
  
  virtual void FocusStateChanged(bool is_focused) {}

 protected:
  IBusPanelCandidateWindowHandlerInterface() {}
};


class UI_EXPORT IBusBridge {
 public:
  virtual ~IBusBridge();

  
  static void Initialize();

  
  static void Shutdown();

  
  static IBusBridge* Get();

  
  
  virtual IBusInputContextHandlerInterface* GetInputContextHandler() const = 0;

  
  
  virtual void SetInputContextHandler(
      IBusInputContextHandlerInterface* handler) = 0;

  
  
  virtual void SetEngineHandler(
      const std::string& engine_id,
      IBusEngineHandlerInterface* handler) = 0;

  
  
  virtual IBusEngineHandlerInterface* GetEngineHandler(
      const std::string& engine_id) = 0;

  
  
  virtual void SetCurrentEngineHandler(IBusEngineHandlerInterface* handler) = 0;

  
  
  
  virtual IBusEngineHandlerInterface* SetCurrentEngineHandlerById(
      const std::string& engine_id) = 0;

  
  
  virtual IBusEngineHandlerInterface* GetCurrentEngineHandler() const = 0;

  
  
  virtual IBusPanelCandidateWindowHandlerInterface*
      GetCandidateWindowHandler() const = 0;

  
  
  virtual void SetCandidateWindowHandler(
      IBusPanelCandidateWindowHandlerInterface* handler) = 0;

 protected:
  IBusBridge();

 private:
  DISALLOW_COPY_AND_ASSIGN(IBusBridge);
};

}  

#endif  
