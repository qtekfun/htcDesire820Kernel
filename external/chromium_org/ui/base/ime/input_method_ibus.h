// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_IBUS_H_
#define UI_BASE_IME_INPUT_METHOD_IBUS_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/base/ime/chromeos/character_composer.h"
#include "ui/base/ime/chromeos/ibus_bridge.h"
#include "ui/base/ime/composition_text.h"
#include "ui/base/ime/input_method_base.h"

namespace dbus {
class ObjectPath;
}
namespace chromeos {
namespace ibus {
class IBusText;
}  
}  

namespace ui {

class UI_EXPORT InputMethodIBus
    : public InputMethodBase,
      public chromeos::IBusInputContextHandlerInterface {
 public:
  explicit InputMethodIBus(internal::InputMethodDelegate* delegate);
  virtual ~InputMethodIBus();

  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) OVERRIDE;
  virtual bool DispatchKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual void OnInputLocaleChanged() OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;
  virtual bool IsCandidatePopupOpen() const OVERRIDE;

 protected:
  
  void ExtractCompositionText(const chromeos::IBusText& text,
                              uint32 cursor_position,
                              CompositionText* out_composition) const;

  
  virtual void ProcessKeyEventPostIME(const ui::KeyEvent& event,
                                      bool handled);

  
  void ResetContext();

 private:
  class PendingKeyEvent;

  
  virtual void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                         TextInputClient* focused) OVERRIDE;
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

  
  void ConfirmCompositionText();

  
  
  void UpdateContextFocusState();

  
  
  void ProcessFilteredKeyPressEvent(const ui::KeyEvent& event);

  
  void ProcessUnfilteredKeyPressEvent(const ui::KeyEvent& event);

  
  
  void ProcessInputMethodResult(const ui::KeyEvent& event, bool filtered);

  
  
  bool NeedInsertChar() const;

  
  bool HasInputMethodResult() const;

  
  
  void AbandonAllPendingKeyEvents();

  
  
  bool ExecuteCharacterComposer(const ui::KeyEvent& event);

  
  virtual void CommitText(const std::string& text) OVERRIDE;
  virtual void UpdatePreeditText(const chromeos::IBusText& text,
                                 uint32 cursor_pos,
                                 bool visible) OVERRIDE;
  virtual void DeleteSurroundingText(int32 offset, uint32 length) OVERRIDE;

  
  void HidePreeditText();

  
  void ProcessKeyEventDone(uint32 id, ui::KeyEvent* event, bool is_handled);

  
  
  
  std::set<uint32> pending_key_events_;

  
  
  
  CompositionText composition_;

  
  
  
  string16 result_text_;

  string16 previous_surrounding_text_;
  gfx::Range previous_selection_range_;

  
  bool context_focused_;

  
  bool composing_text_;

  
  bool composition_changed_;

  
  
  bool suppress_next_result_;

  
  uint32 current_keyevent_id_;

  
  
  CharacterComposer character_composer_;

  TextInputType previous_textinput_type_;

  
  base::WeakPtrFactory<InputMethodIBus> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodIBus);
};

}  

#endif  
