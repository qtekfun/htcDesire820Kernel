// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_H_
#define UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_H_

#include "build/build_config.h"

#include <set>
#include <string>

#if defined(OS_WIN)
#include <objidl.h>
#elif defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#endif

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/dragdrop/download_file_interface.h"
#include "ui/base/ui_export.h"

class GURL;
class Pickle;

namespace gfx {
class ImageSkia;
class Vector2d;
}

namespace ui {


class UI_EXPORT OSExchangeData {
 public:
  
  // nodes are written using a CustomFormat.
  
  typedef Clipboard::FormatType CustomFormat;

  
  enum Format {
    STRING         = 1 << 0,
    URL            = 1 << 1,
    FILE_NAME      = 1 << 2,
    PICKLED_DATA   = 1 << 3,
#if defined(OS_WIN)
    FILE_CONTENTS  = 1 << 4,
#endif
#if defined(OS_WIN) || defined(USE_AURA)
    HTML           = 1 << 5,
#endif
  };

  
  
  enum FilenameToURLPolicy { CONVERT_FILENAMES, DO_NOT_CONVERT_FILENAMES, };

  
  struct UI_EXPORT DownloadFileInfo {
    DownloadFileInfo(const base::FilePath& filename,
                     DownloadFileProvider* downloader);
    ~DownloadFileInfo();

    base::FilePath filename;
    scoped_refptr<DownloadFileProvider> downloader;
  };

  
  struct UI_EXPORT FileInfo {
    FileInfo(const base::FilePath& path, const base::FilePath& display_name);
    ~FileInfo();

    
    base::FilePath path;
    
    base::FilePath display_name;
  };

  
  
  class UI_EXPORT Provider {
   public:
    Provider() {}
    virtual ~Provider() {}

    virtual Provider* Clone() const = 0;

    virtual void SetString(const base::string16& data) = 0;
    virtual void SetURL(const GURL& url, const base::string16& title) = 0;
    virtual void SetFilename(const base::FilePath& path) = 0;
    virtual void SetFilenames(
        const std::vector<FileInfo>& file_names) = 0;
    virtual void SetPickledData(const CustomFormat& format,
                                const Pickle& data) = 0;

    virtual bool GetString(base::string16* data) const = 0;
    virtual bool GetURLAndTitle(FilenameToURLPolicy policy,
                                GURL* url,
                                base::string16* title) const = 0;
    virtual bool GetFilename(base::FilePath* path) const = 0;
    virtual bool GetFilenames(
        std::vector<FileInfo>* file_names) const = 0;
    virtual bool GetPickledData(const CustomFormat& format,
                                Pickle* data) const = 0;

    virtual bool HasString() const = 0;
    virtual bool HasURL() const = 0;
    virtual bool HasFile() const = 0;
    virtual bool HasCustomFormat(const CustomFormat& format) const = 0;

#if defined(OS_WIN)
    virtual void SetFileContents(const base::FilePath& filename,
                                 const std::string& file_contents) = 0;
    virtual bool GetFileContents(base::FilePath* filename,
                                 std::string* file_contents) const = 0;
    virtual bool HasFileContents() const = 0;
    virtual void SetDownloadFileInfo(const DownloadFileInfo& download) = 0;
    virtual void SetInDragLoop(bool in_drag_loop) = 0;
#endif

#if defined(OS_WIN) || defined(USE_AURA)
    virtual void SetHtml(const base::string16& html, const GURL& base_url) = 0;
    virtual bool GetHtml(base::string16* html, GURL* base_url) const = 0;
    virtual bool HasHtml() const = 0;
#endif

#if defined(USE_AURA)
    virtual void SetDragImage(const gfx::ImageSkia& image,
                              const gfx::Vector2d& cursor_offset) = 0;
    virtual const gfx::ImageSkia& GetDragImage() const = 0;
    virtual const gfx::Vector2d& GetDragImageOffset() const = 0;
#endif
  };

  
  static Provider* CreateProvider();

  OSExchangeData();
  
  
  explicit OSExchangeData(Provider* provider);

  ~OSExchangeData();

  
  const Provider& provider() const { return *provider_; }
  Provider& provider() { return *provider_; }

  
  
  
  
  
  
  
  
  
  
  void SetString(const base::string16& data);
  
  void SetURL(const GURL& url, const base::string16& title);
  
  void SetFilename(const base::FilePath& path);
  
  void SetFilenames(
      const std::vector<FileInfo>& file_names);
  
  void SetPickledData(const CustomFormat& format, const Pickle& data);

  
  
  
  
  bool GetString(base::string16* data) const;
  bool GetURLAndTitle(FilenameToURLPolicy policy,
                      GURL* url,
                      base::string16* title) const;
  
  bool GetFilename(base::FilePath* path) const;
  bool GetFilenames(
      std::vector<FileInfo>* file_names) const;
  bool GetPickledData(const CustomFormat& format, Pickle* data) const;

  
  
  bool HasString() const;
  bool HasURL() const;
  bool HasFile() const;
  bool HasCustomFormat(const CustomFormat& format) const;

  
  
  bool HasAllFormats(int formats,
                     const std::set<CustomFormat>& custom_formats) const;

  
  
  bool HasAnyFormat(int formats,
                     const std::set<CustomFormat>& custom_formats) const;

#if defined(OS_WIN)
  
  void SetFileContents(const base::FilePath& filename,
                       const std::string& file_contents);
  bool GetFileContents(base::FilePath* filename,
                       std::string* file_contents) const;

  
  void SetDownloadFileInfo(const DownloadFileInfo& download);

  void SetInDragLoop(bool in_drag_loop);
#endif

#if defined(OS_WIN) || defined(USE_AURA)
  
  
  void SetHtml(const base::string16& html, const GURL& base_url);
  bool GetHtml(base::string16* html, GURL* base_url) const;
#endif

 private:
  
  scoped_ptr<Provider> provider_;

  DISALLOW_COPY_AND_ASSIGN(OSExchangeData);
};

}  

#endif  
