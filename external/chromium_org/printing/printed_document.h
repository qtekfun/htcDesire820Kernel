// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_PRINTED_DOCUMENT_H_
#define PRINTING_PRINTED_DOCUMENT_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "printing/print_settings.h"
#include "ui/gfx/native_widget_types.h"

namespace base {
class FilePath;
class MessageLoop;
}

namespace printing {

class Metafile;
class PrintedPage;
class PrintedPagesSource;
class PrintingContext;

class PRINTING_EXPORT PrintedDocument
    : public base::RefCountedThreadSafe<PrintedDocument> {
 public:
  
  
  PrintedDocument(const PrintSettings& settings,
                  PrintedPagesSource* source,
                  int cookie);

  
  
  void SetPage(int page_number, Metafile* metafile, double shrink,
               const gfx::Size& paper_size, const gfx::Rect& page_rect);

  
  
  
  bool GetPage(int page_number, scoped_refptr<PrintedPage>* page);

  
  
#if defined(OS_WIN) || defined(OS_MACOSX) && !defined(USE_AURA)
  void RenderPrintedPage(const PrintedPage& page,
                         gfx::NativeDrawingContext context) const;
#elif defined(OS_POSIX)
  void RenderPrintedPage(const PrintedPage& page,
                         PrintingContext* context) const;
#endif

  
  
  
  bool IsComplete() const;

  
  
  void DisconnectSource();

  
  
  uint32 MemoryUsage() const;

  
  
  
  void set_page_count(int max_page);

  
  
  int page_count() const;

  
  
  
  
  int expected_page_count() const;

  
  const PrintSettings& settings() const { return immutable_.settings_; }
  const base::string16& name() const { return immutable_.name_; }
  int cookie() const { return immutable_.cookie_; }

  
  
  static void set_debug_dump_path(const base::FilePath& debug_dump_path);

  static const base::FilePath& debug_dump_path();

 private:
  friend class base::RefCountedThreadSafe<PrintedDocument>;

  virtual ~PrintedDocument();

  
  typedef std::map<int, scoped_refptr<PrintedPage> > PrintedPages;

  
  
  struct Mutable {
    explicit Mutable(PrintedPagesSource* source);
    ~Mutable();

    
    
    PrintedPagesSource* source_;

    
    
    PrintedPages pages_;

    
    
    int expected_page_count_;

    
    int page_count_;

#if defined(OS_POSIX) && !defined(OS_MACOSX)
    
    int first_page;
#endif
  };

  
  
  
  struct Immutable {
    Immutable(const PrintSettings& settings, PrintedPagesSource* source,
              int cookie);
    ~Immutable();

    
    PrintSettings settings_;

    
    base::MessageLoop* source_message_loop_;

    
    base::string16 name_;

    
    
    
    
    
    
    int cookie_;
  };

  void DebugDump(const PrintedPage& page);

  
  
  mutable base::Lock lock_;

  
  Mutable mutable_;

  
  const Immutable immutable_;

  DISALLOW_COPY_AND_ASSIGN(PrintedDocument);
};

}  

#endif  
