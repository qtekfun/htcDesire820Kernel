// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_BASE_CLIPBOARD_SCOPED_CLIPBOARD_WRITER_H_
#define UI_BASE_CLIPBOARD_SCOPED_CLIPBOARD_WRITER_H_

#include <string>

#include "base/strings/string16.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/ui_export.h"

class Pickle;

namespace ui {

class UI_EXPORT ScopedClipboardWriter {
 public:
  
  ScopedClipboardWriter(Clipboard* clipboard, ClipboardType type);

  ~ScopedClipboardWriter();

  
  void WriteText(const base::string16& text);

  
  
  void WriteURL(const base::string16& text);

  
  
  void WriteHTML(const base::string16& markup, const std::string& source_url);

  
  void WriteRTF(const std::string& rtf_data);

  
  void WriteBookmark(const base::string16& bookmark_title,
                     const std::string& url);

  
  
  void WriteHyperlink(const base::string16& anchor_text,
                      const std::string& url);

  
  void WriteWebSmartPaste();

  
  void WritePickledData(const Pickle& pickle,
                        const Clipboard::FormatType& format);

  // Removes all objects that would be written to the clipboard.
  void Reset();

 protected:
  
  
  void WriteTextOrURL(const base::string16& text, bool is_url);

  
  
  Clipboard::ObjectMap objects_;
  Clipboard* clipboard_;
  ClipboardType type_;

  
  
  std::string url_text_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedClipboardWriter);
};

}  

#endif  

