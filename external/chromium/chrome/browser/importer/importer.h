// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class ImporterBridge;

namespace importer {
struct SourceProfile;
}

class Importer : public base::RefCountedThreadSafe<Importer> {
 public:
  
  
  
  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) = 0;

  
  virtual void Cancel();

  void set_import_to_bookmark_bar(bool import_to_bookmark_bar) {
    import_to_bookmark_bar_ = import_to_bookmark_bar;
  }

  void set_bookmark_bar_disabled(bool bookmark_bar_disabled) {
    bookmark_bar_disabled_ = bookmark_bar_disabled;
  }

  bool bookmark_bar_disabled() { return bookmark_bar_disabled_; }
  bool cancelled() const { return cancelled_; }

 protected:
  friend class base::RefCountedThreadSafe<Importer>;

  Importer();
  virtual ~Importer();

  
  
  
  static bool ReencodeFavicon(const unsigned char* src_data,
                              size_t src_len,
                              std::vector<unsigned char>* png_data);

  bool import_to_bookmark_bar() const { return import_to_bookmark_bar_; }

  scoped_refptr<ImporterBridge> bridge_;

 private:
  
  bool import_to_bookmark_bar_;

  
  
  
  bool bookmark_bar_disabled_;

  
  bool cancelled_;

  DISALLOW_COPY_AND_ASSIGN(Importer);
};

#endif  
