// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PEM_TOKENIZER_H_
#define NET_BASE_PEM_TOKENIZER_H_
#pragma once

#include <string>
#include <vector>

#include "base/string_piece.h"

namespace net {

class PEMTokenizer {
 public:
  
  
  
  PEMTokenizer(const base::StringPiece& str,
               const std::vector<std::string>& allowed_block_types);
  ~PEMTokenizer();

  
  
  
  bool GetNext();

  
  
  
  const std::string& block_type() const { return block_type_; }

  
  
  
  const std::string& data() const { return data_; }

 private:
  void Init(const base::StringPiece& str,
            const std::vector<std::string>& allowed_block_types);

  
  
  struct PEMType;

  
  
  base::StringPiece str_;

  
  
  base::StringPiece::size_type pos_;

  
  
  
  std::string block_type_;

  
  
  std::vector<PEMType> block_types_;

  
  std::string data_;

  DISALLOW_COPY_AND_ASSIGN(PEMTokenizer);
};

}  

#endif  
