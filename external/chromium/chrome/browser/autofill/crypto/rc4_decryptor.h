// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_CRYPTO_RC4_DECRYPTOR_H_
#define CHROME_BROWSER_AUTOFILL_CRYPTO_RC4_DECRYPTOR_H_

#include <string>
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class RC4Decryptor {
 public:
  explicit RC4Decryptor(wchar_t const* password) {
    PrepareKey(reinterpret_cast<const uint8 *>(password),
               wcslen(password) * sizeof(wchar_t));
    std::wstring data;
    
    data.resize(128, L' ');
    Run(data.c_str());
  }

  
  std::wstring Run(const std::wstring& data) {
    int data_size = data.length() * sizeof(wchar_t);

    scoped_array<wchar_t> buffer(new wchar_t[data.length() + 1]);
    memset(buffer.get(), 0, (data.length() + 1) * sizeof(wchar_t));
    memcpy(buffer.get(), data.c_str(), data_size);

    RunInternal(reinterpret_cast<uint8 *>(buffer.get()), data_size);

    std::wstring result(buffer.get());

    
    memset(buffer.get(), 0, data_size);
    return result;
  }

 private:
  static const int kKeyDataSize = 256;
  struct Rc4Key {
    uint8 state[kKeyDataSize];
    uint8 x;
    uint8 y;
  };

  void SwapByte(uint8* byte1, uint8* byte2) {
    uint8 temp = *byte1;
    *byte1 = *byte2;
    *byte2 = temp;
  }

  void PrepareKey(const uint8 *key_data, int key_data_len) {
    uint8 index1 = 0;
    uint8 index2 = 0;
    uint8* state;
    short counter;

    state = &key_.state[0];
    for (counter = 0; counter < kKeyDataSize; ++counter)
      state[counter] = static_cast<uint8>(counter);

    key_.x = key_.y = 0;

    for (counter = 0; counter < kKeyDataSize; counter++) {
      index2 = (key_data[index1] + state[counter] + index2) % kKeyDataSize;
      SwapByte(&state[counter], &state[index2]);
      index1 = (index1 + 1) % key_data_len;
    }
  }

  void RunInternal(uint8 *buffer, int buffer_len) {
    uint8 x, y;
    uint8 xor_index = 0;
    uint8* state;
    int counter;

    x = key_.x;
    y = key_.y;
    state = &key_.state[0];
    for (counter = 0; counter < buffer_len; ++counter) {
      x = (x + 1) % kKeyDataSize;
      y = (state[x] + y) % kKeyDataSize;
      SwapByte(&state[x], &state[y]);
      xor_index = (state[x] + state[y]) % kKeyDataSize;
      buffer[counter] ^= state[xor_index];
    }
    key_.x = x;
    key_.y = y;
  }

  Rc4Key key_;
};

#endif  
