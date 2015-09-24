// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_CLIPBOARD_CLIENT_H_
#define CONTENT_RENDERER_CLIPBOARD_CLIENT_H_

#include "ui/base/clipboard/clipboard.h"

class GURL;

namespace content {

class ClipboardClient {
 public:
  class WriteContext {
   public:
    virtual ~WriteContext() { }

    
    virtual void WriteBitmapFromPixels(ui::Clipboard::ObjectMap* objects,
                                       const void* pixels,
                                       const gfx::Size& size) = 0;

    
    virtual void Flush(const ui::Clipboard::ObjectMap& objects) = 0;
  };

  virtual ~ClipboardClient() { }

  
  virtual ui::Clipboard* GetClipboard() = 0;

  
  virtual uint64 GetSequenceNumber(ui::ClipboardType type) = 0;

  
  virtual bool IsFormatAvailable(const ui::Clipboard::FormatType& format,
                                 ui::ClipboardType type) = 0;

  
  virtual void Clear(ui::ClipboardType type) = 0;

  
  virtual void ReadAvailableTypes(ui::ClipboardType type,
                                  std::vector<base::string16>* types,
                                  bool* contains_filenames) = 0;

  
  virtual void ReadText(ui::ClipboardType type,
                        base::string16* result) = 0;

  
  virtual void ReadAsciiText(ui::ClipboardType type,
                             std::string* result) = 0;

  
  virtual void ReadHTML(ui::ClipboardType type,
                        base::string16* markup,
                        GURL* url,
                        uint32* fragment_start,
                        uint32* fragment_end) = 0;

  
  virtual void ReadRTF(ui::ClipboardType type, std::string* result) = 0;

  
  virtual void ReadImage(ui::ClipboardType type, std::string* data) = 0;

  
  virtual void ReadCustomData(ui::ClipboardType clipboard_type,
                              const base::string16& type,
                              base::string16* data) = 0;

  
  virtual void ReadData(const ui::Clipboard::FormatType& format,
                        std::string* data) = 0;

  
  virtual WriteContext* CreateWriteContext() = 0;
};

}  

#endif  

