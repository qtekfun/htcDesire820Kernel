// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PRINTING_EMF_WIN_H_
#define PRINTING_EMF_WIN_H_

#include <windows.h>

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "printing/metafile.h"

namespace base {
class FilePath;
}

namespace gfx {
class Rect;
class Size;
}

namespace printing {

const size_t kMetafileMaxSize = 350*1024*1024;

class PRINTING_EXPORT Emf : public Metafile {
 public:
  class Record;
  class Enumerator;
  struct EnumerationContext;

  
  
  Emf();
  virtual ~Emf();

  
  
  virtual bool InitToFile(const base::FilePath& metafile_path);

  
  virtual bool InitFromFile(const base::FilePath& metafile_path);

  
  virtual bool Init() OVERRIDE;
  virtual bool InitFromData(const void* src_buffer,
                            uint32 src_buffer_size) OVERRIDE;

  virtual SkBaseDevice* StartPageForVectorCanvas(
      const gfx::Size& page_size, const gfx::Rect& content_area,
      const float& scale_factor) OVERRIDE;
  
  
  
  
  virtual bool StartPage(const gfx::Size& page_size,
                         const gfx::Rect& content_area,
                         const float& scale_factor) OVERRIDE;
  virtual bool FinishPage() OVERRIDE;
  virtual bool FinishDocument() OVERRIDE;

  virtual uint32 GetDataSize() const OVERRIDE;
  virtual bool GetData(void* buffer, uint32 size) const OVERRIDE;

  
  
  
  virtual bool SaveTo(const base::FilePath& file_path) const OVERRIDE;

  
  virtual gfx::Rect GetPageBounds(unsigned int page_number) const OVERRIDE;

  virtual unsigned int GetPageCount() const OVERRIDE {
    return page_count_;
  }

  virtual HDC context() const OVERRIDE {
    return hdc_;
  }

  virtual bool Playback(HDC hdc, const RECT* rect) const OVERRIDE;
  virtual bool SafePlayback(HDC hdc) const OVERRIDE;

  virtual HENHMETAFILE emf() const OVERRIDE {
    return emf_;
  }

  
  bool IsAlphaBlendUsed() const;

  
  
  Emf* RasterizeMetafile(int raster_area_in_pixels) const;

  
  
  Emf* RasterizeAlphaBlend() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(EmfTest, DC);
  FRIEND_TEST_ALL_PREFIXES(EmfPrintingTest, PageBreak);
  FRIEND_TEST_ALL_PREFIXES(EmfTest, FileBackedEmf);

  
  bool GetDataAsVector(std::vector<uint8>* buffer) const;

  
  static int CALLBACK SafePlaybackProc(HDC hdc,
                                       HANDLETABLE* handle_table,
                                       const ENHMETARECORD* record,
                                       int objects_count,
                                       LPARAM param);

  
  HENHMETAFILE emf_;

  
  HDC hdc_;

  int page_count_;

  DISALLOW_COPY_AND_ASSIGN(Emf);
};

struct Emf::EnumerationContext {
  EnumerationContext();

  HANDLETABLE* handle_table;
  int objects_count;
  HDC hdc;
  const XFORM* base_matrix;
  int dc_on_page_start;
};

class PRINTING_EXPORT Emf::Record {
 public:
  
  bool Play(EnumerationContext* context) const;

  
  
  bool SafePlayback(EnumerationContext* context) const;

  
  const ENHMETARECORD* record() const { return record_; }

 protected:
  explicit Record(const ENHMETARECORD* record);

 private:
  friend class Emf;
  friend class Enumerator;
  const ENHMETARECORD* record_;
};

class PRINTING_EXPORT Emf::Enumerator {
 public:
  
  typedef std::vector<Record>::const_iterator const_iterator;

  
  
  
  Enumerator(const Emf& emf, HDC hdc, const RECT* rect);

  
  const_iterator begin() const;

  
  const_iterator end() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(EmfPrintingTest, Enumerate);

  
  static int CALLBACK EnhMetaFileProc(HDC hdc,
                                      HANDLETABLE* handle_table,
                                      const ENHMETARECORD* record,
                                      int objects_count,
                                      LPARAM param);

  
  
  
  std::vector<Record> items_;

  EnumerationContext context_;

  DISALLOW_COPY_AND_ASSIGN(Enumerator);
};

}  

#endif  
