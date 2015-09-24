// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_CHROMEOS_CHARACTER_COMPOSER_H_
#define UI_BASE_IME_CHROMEOS_CHARACTER_COMPOSER_H_

#include <vector>

#include "base/strings/string_util.h"
#include "ui/base/ui_export.h"

namespace ui {
class KeyEvent;

class UI_EXPORT CharacterComposer {
 public:
  CharacterComposer();
  ~CharacterComposer();

  void Reset();

  
  
  
  
  bool FilterKeyPress(const ui::KeyEvent& event);

  
  
  const string16& composed_character() const { return composed_character_; }

  
  const string16& preedit_string() const { return preedit_string_; }

 private:
  friend class CharacterComposerTest;

  
  enum CompositionMode {
    
    
    KEY_SEQUENCE_MODE,
    
    HEX_MODE,
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool FilterKeyPressInternal(unsigned int keyval, unsigned int keycode,
                              int flags);

  
  bool FilterKeyPressSequenceMode(unsigned int keyval, int flags);

  
  bool FilterKeyPressHexMode(unsigned int keyval, unsigned int keycode,
                             int flags);

  
  void CommitHex();

  
  void UpdatePreeditStringHexMode();

  
  std::vector<unsigned int> compose_buffer_;

  
  string16 composed_character_;

  
  string16 preedit_string_;

  
  CompositionMode composition_mode_;

  DISALLOW_COPY_AND_ASSIGN(CharacterComposer);
};

}  

#endif  
