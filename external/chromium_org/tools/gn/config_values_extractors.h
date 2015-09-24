// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_CONFIG_VALUES_EXTRACTORS_H_
#define TOOLS_GN_CONFIG_VALUES_EXTRACTORS_H_

#include <ostream>
#include <string>
#include <vector>

#include "tools/gn/config.h"
#include "tools/gn/config_values.h"
#include "tools/gn/target.h"

struct EscapeOptions;

class ConfigValuesIterator {
 public:
  explicit ConfigValuesIterator(const Target* target)
      : target_(target),
        cur_index_(-1) {
  }

  bool done() const {
    return cur_index_ >= static_cast<int>(target_->configs().size());
  }

  const ConfigValues& cur() const {
    if (cur_index_ == -1)
      return target_->config_values();
    return target_->configs()[cur_index_].ptr->config_values();
  }

  
  
  const ParseNode* origin() const {
    if (cur_index_ == -1)
      return NULL;
    return target_->configs()[cur_index_].origin;
  }

  void Next() {
    cur_index_++;
  }

  
  
  const Config* GetCurrentConfig() const {
    if (cur_index_ == -1)
      return NULL;
    return target_->configs()[cur_index_].ptr;
  }

 private:
  const Target* target_;

  
  
  int cur_index_;
};

template<typename T, class Writer>
inline void ConfigValuesToStream(
    const ConfigValues& values,
    const std::vector<T>& (ConfigValues::* getter)() const,
    const Writer& writer,
    std::ostream& out) {
  const std::vector<T>& v = (values.*getter)();
  for (size_t i = 0; i < v.size(); i++)
    writer(v[i], out);
};

template<typename T, class Writer>
inline void RecursiveTargetConfigToStream(
    const Target* target,
    const std::vector<T>& (ConfigValues::* getter)() const,
    const Writer& writer,
    std::ostream& out) {
  for (ConfigValuesIterator iter(target); !iter.done(); iter.Next())
    ConfigValuesToStream(iter.cur(), getter, writer, out);
}

void RecursiveTargetConfigStringsToStream(
    const Target* target,
    const std::vector<std::string>& (ConfigValues::* getter)() const,
    const EscapeOptions& escape_options,
    std::ostream& out);

#endif  
